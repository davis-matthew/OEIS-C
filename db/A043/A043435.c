/*
 * A043435  Numbers having three 3's in base 8.
 * https://oeis.org/A043435
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 37, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers having exactly three 3's in base 8. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* undefined offset – return 0 */

    seq_t val = 0;                     /* candidate number */
    idx_t found = 0;                   /* how many have been found */

    while (1) {
        seq_t t = val;
        int cnt = 0;

        /* Count digits equal to 3 in base 8 representation. */
        while (t != 0) {
            if ((t & 7) == 3) {        /* digit = t % 8 */
                ++cnt;
                if (cnt > 3) break;    /* early exit if too many */
            }
            t >>= 3;                    /* t /= 8 */
        }

        if (cnt == 3) {
            ++found;
            if (found == n) return val;
        }

        ++val;                         /* next integer to test */
    }
}
