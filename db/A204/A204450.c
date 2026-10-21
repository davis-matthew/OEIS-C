/*
 * A204450  2*A203579 - A204449. Difference between the exponential
 *          convolution of A000032 (Lucas) with itself and the corresponding
 *          exponential half-convolution.
 * https://oeis.org/A204450
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(2*k) = ((2^(2*k) - binomial(2*k,k)) * L(2*k)) / 2 + 1 - (-1)^k
 *            * binomial(2*k,k); a(2*k+1) = 2^(2*k) * L(2*k+1) + 1 where L(n)
 *            are the Lucas numbers, L(0)=2, L(1)=1
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 19, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/pow2.h"
#include "../utils/binomial.h"

static seq_t *lucas_cache = NULL;
static idx_t lucas_cache_size = 0;

// Return the n-th Lucas number (L(n))
static seq_t lucas(idx_t n)
{
    if (lucas_cache_size <= n) {
        idx_t new_size = lucas_cache_size ? lucas_cache_size << 1 : 1;
        while (new_size <= n) new_size <<= 1;
        seq_t *tmp = (seq_t *)realloc(lucas_cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;              /* allocation failure – ignored */
        if (lucas_cache_size == 0) {
            tmp[0] = (seq_t)2;
            if (new_size > 1) tmp[1] = (seq_t)1;
        }
        for (idx_t i = lucas_cache_size; i < new_size; ++i) {
            if (i == 0) tmp[0] = (seq_t)2;
            else if (i == 1) tmp[1] = (seq_t)1;
            else tmp[i] = tmp[i-1] + tmp[i-2];
        }
        lucas_cache = tmp;
        lucas_cache_size = new_size;
    }
    return lucas_cache[n];
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    if (n & 1) {                    // odd n = 2*k+1
        idx_t k = n >> 1;
        seq_t L = lucas(n);                   // L(2*k+1)
        seq_t pow = pow2(2*k);                // 2^(2*k)
        return pow * L + (seq_t)1;
    } else {                         // even n = 2*k
        idx_t k = n >> 1;
        seq_t L      = lucas(n);                     // L(2*k)
        seq_t pow    = pow2(2*k);                     // 2^(2*k)
        seq_t bin    = binomial(2*k, k);              // C(2*k,k)
        seq_t diff   = pow - bin;                     // (2^(2*k) - C(2*k,k))
        seq_t prod   = diff * L;                      // (diff)*L
        seq_t temp   = prod / ((seq_t)2);             // *L/2
        seq_t t      = temp + (seq_t)1;               // temp + 1
        if (k & 1) {
            return t + bin;          // odd k: + bin
        } else {
            return t - bin;          // even k: - bin
        }
    }
}
