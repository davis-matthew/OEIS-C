/*
 * A067306  One-fourth of third column of triangle A067304.
 * https://oeis.org/A067306
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = A067304(n+2, 2)/4
 * Derived from the generating function
 *     G(x) = (3+10c(4x)+3c(4x)^2)c(4x)^3 / (1+c(4x))^4,
 * where c(x) is the Catalan generating function.
 */

#include <stddef.h>

static idx_t cache_size = 0;
static seq_t *a_cache = NULL;
static seq_t *d_cache = NULL;
static seq_t *pow2_cache = NULL;
static seq_t *pow3_cache = NULL;
static seq_t *pow4_cache = NULL;
static seq_t *pow5_cache = NULL;
static seq_t *D_cache = NULL;
static seq_t *N_cache = NULL;

/* helper to ensure cache grows to the needed size */
static void ensure_cache(idx_t n)
{
    if (n < cache_size) return;

    /* free old if any */
    if (a_cache)  free(a_cache);
    if (d_cache)  free(d_cache);
    if (pow2_cache) free(pow2_cache);
    if (pow3_cache) free(pow3_cache);
    if (pow4_cache) free(pow4_cache);
    if (pow5_cache) free(pow5_cache);
    if (D_cache)  free(D_cache);
    if (N_cache)  free(N_cache);

    /* allocate new cache */
    size_t N = (size_t)(n + 1);
    a_cache  = (seq_t*)malloc(N * sizeof(seq_t));
    d_cache  = (seq_t*)malloc(N * sizeof(seq_t));
    pow2_cache = (seq_t*)malloc(N * sizeof(seq_t));
    pow3_cache = (seq_t*)malloc(N * sizeof(seq_t));
    pow4_cache = (seq_t*)malloc(N * sizeof(seq_t));
    pow5_cache = (seq_t*)malloc(N * sizeof(seq_t));
    D_cache  = (seq_t*)malloc(N * sizeof(seq_t));
    N_cache  = (seq_t*)malloc(N * sizeof(seq_t));

    /* compute Catalan numbers times 4^k */
    d_cache[0] = 1;          /* C0 = 1, 4^0 = 1 */
    seq_t cat = 1;           /* current Catalan number */
    seq_t pow4 = 1;          /* current 4^k */

    for (idx_t k = 1; k <= n; ++k) {
        /* Catalan recurrence: Ck = Ck-1 * 2*(2k-1)/(k+1) */
        cat = cat * (2 * (2 * k - 1));
        cat = cat / (k + 1);
        /* 4^k */
        pow4 = pow4 * 4;
        d_cache[k] = cat * pow4;
    }

    size_t Nsize = (size_t)n + 1;

    /* pow2 = d * d */
    for (size_t i = 0; i < Nsize; ++i) {
        pow2_cache[i] = 0;
        for (size_t j = 0; j <= i; ++j) {
            pow2_cache[i] += d_cache[j] * d_cache[i - j];
        }
    }

    /* pow3 = pow2 * d */
    for (size_t i = 0; i < Nsize; ++i) {
        pow3_cache[i] = 0;
        for (size_t j = 0; j <= i; ++j) {
            pow3_cache[i] += pow2_cache[j] * d_cache[i - j];
        }
    }

    /* pow4 = pow3 * d */
    for (size_t i = 0; i < Nsize; ++i) {
        pow4_cache[i] = 0;
        for (size_t j = 0; j <= i; ++j) {
            pow4_cache[i] += pow3_cache[j] * d_cache[i - j];
        }
    }

    /* pow5 = pow4 * d */
    for (size_t i = 0; i < Nsize; ++i) {
        pow5_cache[i] = 0;
        for (size_t j = 0; j <= i; ++j) {
            pow5_cache[i] += pow4_cache[j] * d_cache[i - j];
        }
    }

    /* D = 1 + 4d + 6d^2 + 4d^3 + d^4  (series coefficients) */
    D_cache[0] = 16;   /* (1+d(0))^4 = 2^4 = 16 */
    for (size_t i = 1; i < Nsize; ++i) {
        D_cache[i] = 0;
        D_cache[i] += 4 * d_cache[i];
        D_cache[i] += 6 * pow2_cache[i];
        D_cache[i] += 4 * pow3_cache[i];
        D_cache[i] +=   pow4_cache[i];
    }

    /* N = 3d^3 + 10d^4 + 3d^5 */
    N_cache[0] = 16;   /* constant term: 3+10+3 = 16 */
    for (size_t i = 1; i < Nsize; ++i) {
        N_cache[i] = 0;
        N_cache[i] += 3 * pow3_cache[i];
        N_cache[i] +=10 * pow4_cache[i];
        N_cache[i] += 3 * pow5_cache[i];
    }

    /* compute a via recurrence: a * D = N, D[0]=16 */
    a_cache[0] = N_cache[0] / D_cache[0];   /* should be 1 */
    for (size_t i = 1; i < Nsize; ++i) {
        seq_t temp = N_cache[i];
        for (size_t k = 0; k < i; ++k) {
            temp -= a_cache[k] * D_cache[i - k];
        }
        /* division by 16 is exact */
        a_cache[i] = temp / D_cache[0];
    }

    cache_size = n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return a_cache[n];
}
