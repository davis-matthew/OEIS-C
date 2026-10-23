/*
 * A028686  Pseudo Galois numbers for d=24.
 * https://oeis.org/A028686
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 24^n * Product_{k=0..n-1} (24^n - 24^k)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute 24^n */
    seq_t base = 1;
    for (idx_t i = 0; i < n; ++i)
        base *= (seq_t)24;

    seq_t result = 1;
    seq_t pow_k  = 1;   /* 24^0 */

    for (idx_t k = 0; k < n; ++k) {
        result *= (base - pow_k);
        pow_k  *= (seq_t)24;
    }

    return result * base;   /* multiply by the extra 24^n factor */
}
