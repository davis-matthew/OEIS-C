/*
 * A199562  a(n) = 4*9^n + 1.
 * https://oeis.org/A199562
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*9^n + 1
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute 9^n using binary exponentiation */
    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)9;

    idx_t exp = n;
    while (exp) {
        if (exp & (idx_t)1) {
            result *= base;
        }
        if (exp > 1) {            /* avoid unnecessary squaring when exp==1 */
            base *= base;
        }
        exp >>= (idx_t)1;
    }

    return result * (seq_t)4 + (seq_t)1;
}
