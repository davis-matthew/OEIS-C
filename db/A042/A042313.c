/*
 * A042313  Denominators of continued fraction convergents to sqrt(683).
 * https://oeis.org/A042313
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(0) = 1, a(1) = 7, a(2) = 15, a(3) = 67, a(4) = 216, a(5) =
 *            283, a(6) = 499, a(7) = 12758, a(8) = 13257, a(9) = 26015, a(10)
 *            = 91302, a(11) = 391223, a(12) = 873748, a(13) = 6507459, a(14)
 *            = 339261616, a(15) = 2381338771, a(n) = 340135364 * a(n-14) -
 *            a(n-28) for n > 27. The first 28 terms are ob...
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 15, u64 n <= 31, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* standard headers ----------------------------------------------------- */
#include <stddef.h>   /* size_t */
#include <stdint.h>   /* uint64_t */
#include <stdlib.h>   /* malloc, realloc, free, exit */

/* helper definitions --------------------------------------------------- */
static seq_t *cache = NULL;      /* cache of computed terms */
static size_t cache_cap = 0;     /* allocated capacity (number of elements) */
static idx_t cache_len = 0;      /* number of terms stored (next free index) */

/* ensure that the cache can hold indices up to 'need' (0‑based) */
static void
ensure_capacity(idx_t need)
{
    /* need is a 0‑based index; we need capacity need+1 */
    if ((size_t)need + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        if (new_cap <= (size_t)need + 1)
            new_cap = (size_t)need + 1;

        seq_t *tmp = (seq_t *)realloc(cache,
                                      new_cap * sizeof(seq_t));
        if (!tmp)
            exit(EXIT_FAILURE);   /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* main sequence function ------------------------------------------------*/
seq_t a(idx_t n)
{
    /* constant coefficient used in the recurrence (fits in seq_t) */
    const seq_t C14 = (seq_t)340135364;

    /* ------------------------------------------------------------------ */
    /* 1. Initialise cache with the first 28 terms (continued fraction)  */
    if (cache_len == 0) {
        const idx_t N_INIT = 28;
        ensure_capacity(N_INIT);

        /* Continued‑fraction parameters for √683 */
        const uint64_t A0 = 26;   /* floor(√683) */
        uint64_t m = 0;
        uint64_t d = 1;
        uint64_t a_cf = A0;

        /* recurrence for convergent denominators: k_{-1}=0, k_0=1 */
        seq_t k_pp = 0;   /* k_{-1} */
        seq_t k_p  = 1;   /* k_0  */
        cache[0] = k_p;
        cache_len = 1;

        /* generate coefficients and denominators for i = 1 … 27 */
        for (idx_t i = 1; i < N_INIT; ++i) {
            /* compute next continued‑fraction coefficient */
            m = d * a_cf - m;
            d = (683 - m * m) / d;
            a_cf = (A0 + m) / d;

            /* new denominator */
            seq_t k_cur = (seq_t)a_cf * k_p + k_pp;
            cache[i]   = k_cur;
            k_pp = k_p;
            k_p  = k_cur;
        }
        cache_len = N_INIT;
    }

    /* ------------------------------------------------------------------ */
    /* 2. Extend cache up to requested n using the recurrence           */
    if (n + 1 > cache_len) {
        for (idx_t i = cache_len; i <= n; ++i) {
            ensure_capacity(i);
            seq_t prod = cache[i - 14] * C14;
            cache[i]   = prod - cache[i - 28];   /* a(i) = C14*a(i-14) - a(i-28) */
        }
        cache_len = n + 1;
    }

    /* ------------------------------------------------------------------ */
    /* 3. Return required term */
    return cache[n];
}
