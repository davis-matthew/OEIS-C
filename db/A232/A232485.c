/*
 * A232485  a(1) = 3; thereafter a(n+1) = a(n) + product of digits of a(n).
 * https://oeis.org/A232485
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1) = 3; thereafter a(n+1) = a(n) + product of digits of a(n)
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n == 0, return 0 */
    seq_t val = 3;                  /* a(1) */
    for (idx_t i = 1; i < n; ++i) {
        /* compute product of digits of val */
        seq_t prod = 1;
        if (val == 0) {
            prod = 0;
        } else {
            seq_t temp = val;
            while (temp > 0) {
                prod *= (temp % 10);
                temp /= 10;
            }
        }
        val += prod;                /* compute next term */
        if (val == 102) return 102; /* from now on the value stays 102 */
    }
    return val;
}
