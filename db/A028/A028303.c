/*
 * A028303  a(n) = floor( binomial(n, floor(n/2))/(1 + ceiling(n/2)) )
 *          (interpolates between Catalan numbers).
 * https://oeis.org/A028303
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor( binomial(n, floor(n/2)) / (1 + ceil(n/2)) )
 * Range:     u8 n <= 13, u16 n <= 22, u32 n <= 39, u64 n <= 72, u128 n <= 137,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
#if defined(__cplusplus)
    /* C++ build – seq_t supports arbitrary precision, so we can use the
       provided binomial routine directly. */
    idx_t k = n / 2;            /* floor(n/2) */
    idx_t ceil_half = n - k;    /* ceil(n/2)      */
    seq_t denom = (seq_t)(1 + ceil_half);
    seq_t bin = binomial(n, k); /* exact binomial */
    return bin / denom;
#else
    /* C build – seq_t is an unsigned integer type (uint8_t … uint64_t).
       For n that fit into 64 bits we compute the binomial with 128‑bit
       intermediates to avoid overflow, then divide. */
    idx_t k = n / 2;
    idx_t ceil_half = n - k;
    unsigned __int128 bin = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        /* multiply by the next numerator */
        bin = bin * (unsigned __int128)(n - k + i);
        /* divide by i – always exact at this stage */
        bin = bin / (unsigned __int128)i;
    }
    unsigned __int128 denom = (unsigned __int128)(1 + ceil_half);
    unsigned __int128 result = bin / denom;
    return (seq_t)result;
#endif
}
