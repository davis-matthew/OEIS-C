/*
 * A050916  a(n) = n*5^n + 1.
 * https://oeis.org/A050916
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*5^n + 1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 25, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute 5^n using exponentiation by squaring. */
    seq_t result = 1;   /* accumulator for 5^n */
    seq_t base   = 5;   /* current power of 5 (initially 5^1) */
    idx_t  exp   = n;   /* exponent to be reduced */

    while (exp) {
        if (exp & 1) {
            result *= base;
        }
        exp >>= 1;
        if (exp) {            /* only square when we'll use the new base */
            base *= base;
        }
    }

    /* Multiply by n and add 1.  Cast n to seq_t before multiplication to
       avoid intermediate 64‑bit overflow on tiny seq_t types. */
    return result * (seq_t)n + 1;
}
