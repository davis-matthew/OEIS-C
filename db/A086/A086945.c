/*
 * A086945  a(n) = 7*10^n - 9.
 * https://oeis.org/A086945
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 7*10^n - 9
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute 10^n using exponentiation by squaring.
       This works for any seq_t (including arbitrary‑precision). */
    seq_t result = 1;
    seq_t base   = 10;

    idx_t exp = n;
    while (exp > 0) {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        if (exp)
            base *= base; /* square only if we will need it later */
    }

    /* Return 7 * 10^n - 9 */
    return (seq_t)7 * result - (seq_t)9;
}
