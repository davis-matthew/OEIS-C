/*
 * A043453  Numbers having one 0 in base 9.
 * https://oeis.org/A043453
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = n-th positive integer whose base‑9 representation contains exactly one digit 0
   OEIS A043453 */
seq_t a(idx_t n)
{
    /* simple enumeration – fast enough for n up to about 10 000 */
    seq_t count = 0;
    seq_t val   = 9;               /* first possible number (10₉) */
    while (1) {
        /* count zero digits in base‑9 */
        int zeros = 0;
        seq_t tmp = val;
        while (tmp > 0) {
            if ((tmp % 9) == 0)   /* digit is 0 */
                ++zeros;
            tmp /= 9;
        }
        if (zeros == 1) {
            ++count;
            if (count == n)
                return val;
        }
        ++val;
    }
}
