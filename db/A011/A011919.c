/*
 * A011919  a(n) = floor(n*(n-1)*(n-2)*(n-3)/9).
 * https://oeis.org/A011919
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(n-1)*(n-2)*(n-3)/9)
 * Range:     u8 n <= 5, u16 n <= 17, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) for A011919.  Works for all supported seq_t sizes,
   including arbitrary-precision types.  For built-in unsigned types
   the product fits in uint64_t for n <= 10000, so the calculation
   is safe. */
seq_t a(idx_t n)
{
    if (n < 4) return 0;
    seq_t x = (seq_t)n;
    seq_t prod = x * (x - 1) * (x - 2) * (x - 3);
    prod /= 9;
    return prod;
}
