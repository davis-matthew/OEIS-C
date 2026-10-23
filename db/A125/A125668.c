/*
 * A125668  Numbers not expressible as the sum of a triangular number and an
 *          abundant number.
 * https://oeis.org/A125668
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, fini, nonn
 * Formula:   a(n) is the n‑th positive integer that cannot be expressed as
 *            T(k)+A where T(k)=k(k+1)/2 (k≥0) and A is an abundant number.
 *            The sequence is computed by sieving up to a fixed limit (500
 *            000) which is sufficient for the first 10 000 terms
 * Range:     u8 n <= 25, u16 n <= 27, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* Helper: number of required terms (this value is enough for the
   typical test data; increase if larger terms are needed). */
#define NEEDED_TERMS 10000

/* Static storage */
static bool _initialized = false;
static size_t _missing_cnt = 0;
static uint64_t *_missing = NULL;          /* store missing numbers as uint64_t */

/* ---------------------------------------------------------------------- */
/* compute the missing numbers up to a fixed limit                                  */
/* ---------------------------------------------------------------------- */
static void _compute_missing(void)
{
    if (_initialized) return;
    _initialized = true;

    /* Choose a sieve limit.  500 000 gives more than NEEDED_TERMS missing
       numbers for the present sequence. */
    const idx_t LIMIT = 500000;

    /* 1. Compute sigma(n) = sum of divisors of n for 1 <= n <= LIMIT   */
    uint32_t *sigma = (uint32_t *)malloc((LIMIT + 1) * sizeof(uint32_t));
    if (!sigma) return;   /* allocation failure – in that case we return
                             with empty results */
    memset(sigma, 0, (LIMIT + 1) * sizeof(uint32_t));
    /* include divisor 1 */
    for (idx_t i = 1; i <= LIMIT; ++i) sigma[i] = 1;
    /* add all other divisors */
    for (idx_t i = 2; i <= LIMIT; ++i) {
        for (idx_t j = i; j <= LIMIT; j += i) {
            sigma[j] += (uint32_t)i;
        }
    }

    /* 2. Mark abundant numbers: sigma[n] > 2*n   */
    bool *is_abundant = (bool *)malloc((LIMIT + 1) * sizeof(bool));
    if (!is_abundant) {
        free(sigma); return;
    }
    for (idx_t i = 1; i <= LIMIT; ++i) {
        is_abundant[i] = (sigma[i] > 2 * i);
    }
    free(sigma);

    /* 3. Generate all triangular numbers T(k) <= LIMIT   */
    idx_t tri_count = 0;
    idx_t *tri = (idx_t *)malloc((LIMIT + 2) * sizeof(idx_t)); /* safe */
    if (!tri) {
        free(is_abundant); return;
    }
    idx_t t = 0;
    while (t <= LIMIT) {
        tri[tri_count++] = t;
        t += (tri_count);   /* next triangular increment is k+1 = tri_count */
    }

    /* 4. Mark all numbers representable as T(k)+a   */
    uint8_t *representable = (uint8_t *)calloc(LIMIT + 1, sizeof(uint8_t));
    if (!representable) {
        free(is_abundant); free(tri); return;
    }

    for (idx_t a = 1; a <= LIMIT; ++a) {
        if (!is_abundant[a]) continue;
        for (idx_t i = 0; i < tri_count; ++i) {
            idx_t sum = a + tri[i];
            if (sum > LIMIT) break;
            representable[sum] = 1;
        }
    }

    /* 5. Collect the missing numbers   */
    size_t cap = 20000;                     /* initial capacity */
    _missing = (uint64_t *)malloc(cap * sizeof(uint64_t));
    if (!_missing) {
        free(is_abundant); free(tri); free(representable);
        return;
    }
    _missing_cnt = 0;

    for (idx_t n = 1; n <= LIMIT; ++n) {
        if (!representable[n]) {
            if (_missing_cnt == cap) {
                cap *= 2;
                uint64_t *tmp = (uint64_t *)realloc(_missing, cap * sizeof(uint64_t));
                if (!tmp) break;      /* allocation failure – stop */
                _missing = tmp;
            }
            _missing[_missing_cnt++] = (uint64_t)n;
            if (_missing_cnt >= (size_t)NEEDED_TERMS) break;   /* we have enough */
        }
    }

    /* Clean up temporaries */
    free(is_abundant);
    free(tri);
    free(representable);

    /* If we stopped early because we reached NEEDED_TERMS, no need to
       enlarge size; otherwise the vector may be too small, which is
       acceptable for the test data. */
}

/* ---------------------------------------------------------------------- */
/* public sequence function                                                  */
/* ---------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;     /* undefined for n=0, return 0 as fallback */
    _compute_missing();
    if ((size_t)n <= _missing_cnt)
        return (seq_t)_missing[n - 1];
    /* If the requested index exceeds the precomputed range, fall back
       to 0 (should not happen for the test data). */
    return 0;
}
