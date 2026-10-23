/*
 * A183038  G.f.: exp( Sum_{n>=1} A051064(n)*3^A051064(n)*x^n/n ) where
 *          A051064(n) equals the 3-adic valuation of 3n.
 * https://oeis.org/A183038
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 24, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A183038
 *
 * a(0) = 1
 * For n ≥ 1:
 *     n * a(n) = ∑_{k=1}^{n} f(k) * a(n − k)
 * where f(k) = (v₃(k)+1) * 3^{v₃(k)+1} and v₃(k) is the 3‑adic valuation of k.
 *
 * The formula is the standard recurrence for
 *   A(x) = exp(∑_{n≥1} f(n) xⁿ / n).
 */
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

static idx_t max_n = 0;                 /* largest n that has been computed */
static seq_t *A    = NULL;              /* cached values of a(n)          */
static uint8_t *v3_cache = NULL;        /* cached 3‑adic valuations      */
static seq_t *f_cache    = NULL;        /* cached f(k) values             */

/* 3⁰ = 1, so pow3[0] = 1.  t will never exceed 32 for n <= 10⁴. */
static seq_t pow3[64] = {1};
static size_t pow3_size = 1;             /* next index to compute        */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;   /* a(0) = 1 */

    /* Grow all caches to accommodate n. */
    if (n > max_n) {
        size_t new_len = (size_t)n + 1;

        A       = (seq_t*)realloc(A,       new_len * sizeof(seq_t));
        v3_cache= (uint8_t*)realloc(v3_cache, new_len * sizeof(uint8_t));
        f_cache = (seq_t*)realloc(f_cache,  new_len * sizeof(seq_t));

        if (!A || !v3_cache || !f_cache) return (seq_t)0; /* allocation failed */

        /* Initialisation of the first element (already handled below). */
        if (max_n == 0) {
            A[0] = (seq_t)1;
            v3_cache[0] = 0;
            f_cache[0] = (seq_t)0;
        }

        /* Pre‑compute v₃(k), 3‑powers and f(k) for all new k. */
        for (idx_t i = max_n + 1; i <= n; ++i) {
            uint8_t val3;
            if (i % 3 == 0)
                val3 = v3_cache[i/3] + 1;
            else
                val3 = 0;
            v3_cache[i] = val3;

            uint8_t t = val3 + 1;   /* t = v₃(k)+1, t ≥ 1 */

            /* Grow pow3 array if necessary. */
            while ((size_t)t >= pow3_size) {
                pow3[pow3_size] = pow3[pow3_size - 1] * (seq_t)3;
                ++pow3_size;
            }

            f_cache[i] = (seq_t)t * pow3[t];
        }

        /* Compute a(i) for all new i using the recurrence. */
        for (idx_t i = max_n + 1; i <= n; ++i) {
#if defined(__SIZEOF_INT128__)
            unsigned __int128 sum128 = 0;
            for (idx_t k = 1; k <= i; ++k) {
                sum128 += (unsigned __int128)f_cache[k] *
                          (unsigned __int128)A[i - k];
            }
            A[i] = (seq_t)(sum128 / (unsigned __int128)i);
#else
            seq_t sum = 0;
            for (idx_t k = 1; k <= i; ++k) {
                sum += f_cache[k] * A[i - k];
            }
            A[i] = sum / (seq_t)i;
#endif
        }

        max_n = n;
    }

    return A[n];
}
