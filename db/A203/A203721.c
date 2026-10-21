/*
 * A203721  Number of (n+1) X 2 0..2 arrays with column and row pair sums
 *          b(i,j)=a(i,j)+a(i,j-1) and c(i,j)=a(i,j)+a(i-1,j) nondecreasing in
 *          column and row directions, respectively.
 * https://oeis.org/A203721
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 22, u64 n <= 49, u128 n <= 104,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A203721
 * Empirical recurrence (order 9):
 *
 * a(n) = 6*a(n-1) - 11*a(n-2) + a(n-3) + 16*a(n-4)
 *        - 10*a(n-5) - 7*a(n-6) + 6*a(n-7) + a(n-8) - a(n-9)
 *
 * initial values:
 * a(1)=32, a(2)=113, a(3)=326, a(4)=888, a(5)=2278,
 * a(6)=5653, a(7)=13665, a(8)=32440, a(9)=75965
 *
 * The function below memoizes the sequence in a static cache.
 * It works for any build that supplies the types seq_t and idx_t
 * (unlimited precision or fixed-width unsigned integers).
 * All arithmetic uses only integer operators as required.
 */

#include <stdlib.h>

static const seq_t init_vals[9] = {
    32ULL, 113ULL, 326ULL, 888ULL, 2278ULL,
    5653ULL, 13665ULL, 32440ULL, 75965ULL
};

static seq_t *cache = NULL;        /* cache[0] = a(1) */
static size_t cache_cap = 0;       /* allocated size of cache array */
static size_t cache_len = 0;       /* number of sequence values currently stored */

/* grow the cache to at least 'need' elements */
static void ensure_cache(size_t need)
{
    if (cache_cap >= need) return;
    size_t newcap = cache_cap ? cache_cap * 2 : 16;
    while (newcap < need) newcap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    if (!tmp) abort();             /* out of memory */
    cache = tmp;
    cache_cap = newcap;
}

/* OEIS sequence A203721 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n=0, just return 0 */

    /* initialise cache if this is the first call */
    if (cache_len == 0) {
        ensure_cache((size_t)n > 9 ? (size_t)n : 9);
        for (size_t i = 0; i < 9; ++i) cache[i] = init_vals[i];
        cache_len = 9;
    }

    /* extend cache to cover up to n if needed */
    if (cache_len < (size_t)n) {
        ensure_cache((size_t)n);
        for (idx_t k = (idx_t)(cache_len + 1); k <= n; ++k) {
            /* compute a(k) from the recurrence */
            seq_t val = 0;
            val += 6 * cache[(size_t)(k - 2)];   /* 6*a(k-1)   */
            val -= 11 * cache[(size_t)(k - 3)];  /* -11*a(k-2) */
            val += 1 * cache[(size_t)(k - 4)];   /* +a(k-3)    */
            val += 16 * cache[(size_t)(k - 5)];  /* +16*a(k-4) */
            val -= 10 * cache[(size_t)(k - 6)];  /* -10*a(k-5) */
            val -= 7 * cache[(size_t)(k - 7)];   /* -7*a(k-6)  */
            val += 6 * cache[(size_t)(k - 8)];   /* +6*a(k-7)  */
            val += 1 * cache[(size_t)(k - 9)];   /* +a(k-8)    */
            val -= 1 * cache[(size_t)(k - 10)];  /* -a(k-9)    */
            cache[(size_t)(k - 1)] = val;        /* store a(k) */
        }
        cache_len = (size_t)n;
    }

    return cache[(size_t)(n - 1)];
}
