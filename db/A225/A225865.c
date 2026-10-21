/*
 * A225865  a(n) = 2^m minus (the total number of distinct subsets of
 *          length-(m-n) binary words that can appear as the factor of a word
 *          of length m, for 0 <= n < m/2).
 * https://oeis.org/A225865
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=1..n+1} (i-1)*L(i) (L(i) = Lyndon words of length
 *            i)
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* static cache state */
    static idx_t cached_limit = 0;          /* maximum index computed so far (i.e. highest i used in tables) */
    static int8_t  *mu   = NULL;            /* Möbius function values */
    static seq_t  *pow2v = NULL;            /* pow2v[k] = 2^k */
    static seq_t  *sumPos = NULL;           /* positive μ contributions for each i */
    static seq_t  *sumNeg = NULL;           /* negative μ contributions for each i */
    static seq_t  *L     = NULL;            /* Lyndon word counts */
    static seq_t  *A     = NULL;            /* cumulative sums a(i-1) = Σ_{k=1}^{i-1} (k-1)L(k) */

    /* we need values up to i = n+1 (inclusive) */
    idx_t need = n + 1;
    if (need > cached_limit) {
        /* enlarge all tables to index 'need' */
        idx_t new_size = need + 1;           /* indices 0..need */

        /* reallocate arrays (C++ requires explicit cast) */
        mu      = (int8_t*)realloc(mu,      sizeof(int8_t)  * (size_t)new_size);
        pow2v   = (seq_t*)realloc(pow2v,   sizeof(seq_t)  * (size_t)new_size);
        sumPos  = (seq_t*)realloc(sumPos,  sizeof(seq_t)  * (size_t)new_size);
        sumNeg  = (seq_t*)realloc(sumNeg,  sizeof(seq_t)  * (size_t)new_size);
        L       = (seq_t*)realloc(L,       sizeof(seq_t)  * (size_t)new_size);
        A       = (seq_t*)realloc(A,       sizeof(seq_t)  * (size_t)new_size);

        /* 1. Möbius sieve (linear) up to 'need' */
        for (idx_t i = 0; i < new_size; ++i) mu[i] = 0;   /* clear previous data */
        if (new_size > 0) mu[1] = 1;
        int *is_comp = (int*)calloc((size_t)new_size, sizeof(int));
        int *primes  = (int*)malloc((size_t)new_size * sizeof(int));
        size_t pc = 0;
        for (idx_t i = 2; i < new_size; ++i) {
            if (!is_comp[i]) {
                primes[pc++] = (int)i;
                mu[i] = -1;
            }
            for (size_t j = 0; j < pc && i * (idx_t)primes[j] < new_size; ++j) {
                idx_t v = i * (idx_t)primes[j];
                is_comp[v] = 1;
                if (i % (idx_t)primes[j] == 0) {
                    mu[v] = 0;
                    break;
                } else
                    mu[v] = -mu[i];
            }
        }
        free(is_comp);
        free(primes);

        /* 2. Precompute powers of two */
        for (idx_t i = 0; i < new_size; ++i)
            pow2v[i] = pow2((idx_t)i);

        /* 3. Initialise sign sums */
        for (idx_t i = 0; i < new_size; ++i) {
            sumPos[i] = 0;
            sumNeg[i] = 0;
        }

        /* 4. Convolution: Σ_{d|m} μ(d)·2^{m/d} */
        for (idx_t d = 1; d < new_size; ++d) {
            int8_t mu_d = mu[d];
            if (mu_d == 0) continue;
            for (idx_t m = d; m < new_size; m += d) {
                seq_t val = pow2v[m / d];
                if (mu_d > 0)
                    sumPos[m] += val;
                else
                    sumNeg[m] += val;
            }
        }

        /* 5. Compute L(i) and cumulative a(k) */
        A[0] = 0;
        for (idx_t i = 1; i < new_size; ++i) {
            seq_t diff = sumPos[i] - sumNeg[i];   /* guaranteed non‑negative */
            L[i] = diff / (seq_t)i;               /* integer division */
            A[i] = A[i-1] + (seq_t)(i-1) * L[i];
        }

        cached_limit = need;
    }

    /* a(n) corresponds to cumulative sum up to index n+1 */
    return A[n + 1];
}
