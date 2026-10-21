/*
 * A070533  a(n) = n^4 mod 15.
 * https://oeis.org/A070533
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^4 mod 15
 * Range:     u8 n <= 329, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute n modulo 15, then raise to the fourth power,
       taking the modulus after each multiplication to keep
       intermediate values small.  This works for any seq_t
       size because the modulus is small and fits in the type. */
    const seq_t mod15 = (seq_t)15;
    seq_t r = (seq_t)n % mod15;       /* n mod 15 */
    seq_t r2 = (r * r) % mod15;       /* (n mod 15)^2 mod 15 */
    seq_t r4 = (r2 * r2) % mod15;     /* (n mod 15)^4 mod 15 */
    return r4;
}
