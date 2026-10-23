/*
 * A144177  a(n) = b(2n-1)^b(2n) where b(3n+1) = floor(n/9) + 2, b(3n+2) = (n
 *          mod 9) + 2, b(3n+3) = b(3n+1)*b(3n+2) for n >= 0.
 * https://oeis.org/A144177
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 8, u64 n <= 23, u128 n <= 28,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A144177: a(n) = b(2n-1)^b(2n) where
   b(3k+1) = floor(k/9)+2,
   b(3k+2) = (k % 9)+2,
   b(3k+3) = b(3k+1)*b(3k+2)           */

#include "../utils/big_pow.h"

/* Compute b(k) for k ≥ 1 */
static inline seq_t b_val(idx_t k)
{
    /* k = 3 * n + offset  (offset = 0,1,2)  */
    idx_t n   = (k - 1) / 3;
    idx_t off = (k - 1) % 3;

    if (off == 0)                /* k = 3n + 1 */
        return (seq_t)(n / 9) + 2;

    if (off == 1)                /* k = 3n + 2 */
        return (seq_t)(n % 9) + 2;

    /* off == 2  → k = 3n + 3  */
    seq_t a = (seq_t)(n / 9) + 2;     /* b(3n+1) */
    seq_t b = (seq_t)(n % 9) + 2;     /* b(3n+2) */
    return a * b;                     /* b(3n+3)   */
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* a(n) = b(2n-1)^b(2n) */
    if (n == 0) return 0;             /* undefined for n=0, but safety */

    idx_t k1 = 2 * n - 1;
    idx_t k2 = 2 * n;

    seq_t base = b_val(k1);
    seq_t exp  = b_val(k2);

    return big_pow(base, exp);
}
