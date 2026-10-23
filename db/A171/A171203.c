/*
 * A171203  G.f. satisfies: A(x) = (1 + x*A(2x))^4.
 * https://oeis.org/A171203
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in (1 + x*A(2x))^4 where A(x) = ∑ a(k)
 *            x^k starts with a(0)=1 This implementation uses incremental
 *            convolution to compute a[n] for all n up to the largest request
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Forward declaration of the required function */
seq_t a(idx_t n);

/* Internal helpers and static cache */
static seq_t *a_cache = NULL;     /* a[0..N-1]   */
static seq_t *C_cache = NULL;     /* C[0..N-1]   */
static seq_t *B_cache = NULL;     /* B[0..N-1]   */
static seq_t *pow2_cache = NULL;  /* pow2[i] = 2^i   */
static size_t cache_size = 0;     /* number of elements currently computed */

/* Ensure that all required values up to index n are computed */
static void ensure_cache(idx_t n)
{
    if (n < cache_size) return;

    size_t new_size = (size_t)n + 1;

    /* Allocate arrays if this is the first call */
    if (cache_size == 0) {
        a_cache     = (seq_t*)malloc(sizeof(seq_t) * new_size);
        C_cache     = (seq_t*)malloc(sizeof(seq_t) * new_size);
        B_cache     = (seq_t*)malloc(sizeof(seq_t) * new_size);
        pow2_cache  = (seq_t*)malloc(sizeof(seq_t) * new_size);

        if (!a_cache || !C_cache || !B_cache || !pow2_cache) {
            /* allocation failed – abort with zero */
            if (a_cache) free(a_cache);
            if (C_cache) free(C_cache);
            if (B_cache) free(B_cache);
            if (pow2_cache) free(pow2_cache);
            a_cache = C_cache = B_cache = pow2_cache = NULL;
            cache_size = 0;
            return;
        }

        /* initial values */
        a_cache[0] = (seq_t)1;
        C_cache[0] = (seq_t)1;
        B_cache[0] = (seq_t)1;
        pow2_cache[0] = (seq_t)1;

        cache_size = 1;
    } else if (new_size > cache_size) {
        /* Reallocate arrays to the new size */
        a_cache    = (seq_t*)realloc(a_cache, sizeof(seq_t) * new_size);
        C_cache    = (seq_t*)realloc(C_cache, sizeof(seq_t) * new_size);
        B_cache    = (seq_t*)realloc(B_cache, sizeof(seq_t) * new_size);
        pow2_cache = (seq_t*)realloc(pow2_cache, sizeof(seq_t) * new_size);
        if (!a_cache || !C_cache || !B_cache || !pow2_cache) {
            /* allocation failed – abort with zero */
            if (a_cache) free(a_cache);
            if (C_cache) free(C_cache);
            if (B_cache) free(B_cache);
            if (pow2_cache) free(pow2_cache);
            a_cache = C_cache = B_cache = pow2_cache = NULL;
            cache_size = 0;
            return;
        }
    }

    /* Compute new entries */
    /* pow2 */
    for (size_t i = cache_size; i < new_size; ++i) {
        pow2_cache[i] = pow2_cache[i-1] << 1;
    }
    /* C, B, a */
    for (size_t i = cache_size; i < new_size; ++i) {
        /* C[i] = (i==0)?1 : a[i-1] * 2^(i-1) */
        if (i == 0) {
            C_cache[i] = (seq_t)1;
        } else {
            C_cache[i] = a_cache[i-1] * pow2_cache[i-1];
        }

        /* B[i] = sum_{k=0}^{i} C[k] * C[i-k] */
        seq_t sumB = 0;
        for (size_t k = 0; k <= i; ++k) {
            sumB += C_cache[k] * C_cache[i-k];
        }
        B_cache[i] = sumB;

        /* a[i] = sum_{k=0}^{i} B[k] * B[i-k] */
        seq_t suma = 0;
        for (size_t k = 0; k <= i; ++k) {
            suma += B_cache[k] * B_cache[i-k];
        }
        a_cache[i] = suma;
    }

    cache_size = new_size;
}

seq_t a(idx_t n)
/* Returns the n-th term of OEIS A171203 (0‑based) */
{
    ensure_cache(n);
    if (cache_size == 0) return (seq_t)0; /* allocation failure */
    return a_cache[(size_t)n];
}
