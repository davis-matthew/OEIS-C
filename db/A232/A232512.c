/*
 * A232512  Number of (n+1)X(5+1) 0..2 arrays with every element next to
 *          itself plus and minus one within the range 0..2 horizontally,
 *          diagonally or antidiagonally, with no adjacent elements equal.
 * https://oeis.org/A232512
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 74, a(2) = 250, a(3) = 1730, ... For n>19 the sequence
 *            satisfies the linear recurrence a(n) = 6*a(n-1)
 *            +10*a(n-2)-35*a(n-3)-84*a(n-4)-100*a(n-5)
 *            +131*a(n-6)+280*a(n-7)+161*a(n-8)-211*a(n-9)-243*a(n-10)
 *            -44*a(n-11)+85*a(n-12)-8*a(n-13)-20*a(n-14)-16*a(n-15)
 *            +6*a(n-16)+4*a(n-17)-1*a(n-18) (o...
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, free */

static const int REC_COEF[18] = {
    6, 10, -35, -84, -100, 131, 280, 161,
   -211, -243, -44, 85, -8, -20, -16,
    6, 4, -1
};

static const size_t REC_ORDER = 18;

/* Initial 19 terms (1‑based).  The cast to seq_t works for all
   integer types defined in the build. */
static const seq_t INIT_TERM[19] = {
    (seq_t)74ULL,
    (seq_t)250ULL,
    (seq_t)1730ULL,
    (seq_t)10118ULL,
    (seq_t)64290ULL,
    (seq_t)399122ULL,
    (seq_t)2523098ULL,
    (seq_t)15907690ULL,
    (seq_t)100603770ULL,
    (seq_t)636270550ULL,
    (seq_t)4026503762ULL,
    (seq_t)25483526018ULL,
    (seq_t)161305309394ULL,
    (seq_t)1021070935782ULL,
    (seq_t)6463641254302ULL,
    (seq_t)40917054779762ULL,
    (seq_t)259021102980902ULL,
    (seq_t)1639712263959994ULL,
    (seq_t)10380090197352550ULL
};

/* Static cache for computed terms.  The cache grows lazily on demand. */
static seq_t   *num_cache = NULL;     /* pointer to the array */
static size_t   cache_size = 0;       /* number of terms stored */
static size_t   cache_cap  = 0;       /* allocated capacity */

static void ensure_cache_size(idx_t n)
{
    if (cache_size >= n)
        return;                       /* already stored up to n */

    /* Allocate enough space for n terms.  If upgrading capacity, use realloc. */
    if (cache_cap < (size_t)n) {
        seq_t *new_ptr = (seq_t *)realloc(num_cache, n * sizeof(seq_t));
        if (!new_ptr) {                /* allocation failure – abort */
            free(num_cache);
            num_cache = NULL;
            cache_cap = 0;
            cache_size = 0;
            return;
        }
        num_cache = new_ptr;
        cache_cap = n;
    }

    /* Fill missing terms in the cache.  The loop is 0‑based; term i corresponds
       to a(i+1). */
    for (size_t idx = cache_size; idx < (size_t)n; ++idx) {
        if (idx < 19) {
            /* Initial terms are known directly. */
            num_cache[idx] = INIT_TERM[idx];
        } else {
            seq_t sum_pos = 0;
            seq_t sum_neg = 0;
            for (int j = 0; j < (int)REC_ORDER; ++j) {
                seq_t term = num_cache[idx - (j + 1)];
                int coeff = REC_COEF[j];
                if (coeff > 0) {
                    sum_pos += term * (seq_t)coeff;
                } else {
                    sum_neg += term * (seq_t)(-coeff);
                }
            }
            /* sum_pos >= sum_neg for all n>19, so subtraction is safe. */
            num_cache[idx] = sum_pos - sum_neg;
        }
    }
    cache_size = n;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* a(0) is undefined; return 0 */
    ensure_cache_size(n);
    return num_cache[n - 1];      /* 1‑based index */
}
