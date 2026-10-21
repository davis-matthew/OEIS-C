/*
 * A061501  a(1) = 1, a(n+1) = (a(n) + n) mod 10.
 * https://oeis.org/A061501
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, less
 * Formula:   a(n) = (n-1)*n/2 + 1 (mod 10)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This uses the recurrence a(1)=1, a(n+1)=(a(n)+n) mod 10.  */
seq_t a(idx_t n)
{
    /* Reduce n modulo 20 first to avoid overflow in intermediate product
       because ((n-1)*n)/2 mod 10 only depends on ((n-1)*n) mod 20. */
    uint64_t n_mod20     = n % 20;
    uint64_t n_minus_1   = (n - 1) % 20;
    uint64_t prod_mod20  = (n_mod20 * n_minus_1) % 20;          /* even */
    uint64_t half        = prod_mod20 / 2;                      /* integer division */
    uint64_t result      = (half + 1) % 10;                    /* final value 0..9 */
    return (seq_t)result;
}
