/*
 * A063653  Number of ways to tile a 9 X n rectangle with 1 X 1 and 2 X 2
 *          tiles.
 * https://oeis.org/A063653
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 19,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ====================================================================== */
/*  OEIS A063653: Number of tilings of a 9×n rectangle with 1×1 and 2×2 tiles
 *
 *  Recurrence (valid for n ≥ 19):
 *
 *    a(n) =  6*a(n-1) + 110*a(n-2) - 262*a(n-3) - 2786*a(n-4)
 *           + 5916*a(n-5) + 25168*a(n-6) - 53907*a(n-7) - 95299*a(n-8)
 *           + 197820*a(n-9) + 193866*a(n-10)
 *           - 340168*a(n-11) - 228528*a(n-12) + 279636*a(n-13)
 *           + 137864*a(n-14) - 108909*a(n-15) - 33736*a(n-16)
 *           + 20214*a(n-17) + 2460*a(n-18) - 1296*a(n-19)
 *
 *  Initial values:
 *
 *    a(0) =   1
 *    a(1) =   1
 *    a(2) =  55
 *    a(3) = 341
 *    a(4) = 5933
 *    a(5) = 59925
 *    a(6) = 795611
 *    a(7) = 9167119
 *    a(8) = 113555791
 *    a(9) = 1355115601
 *   a(10) = 16484061769
 *   a(11) = 198549329897
 *   a(12) = 2403674442213
 *   a(13) = 29023432116879
 *   a(14) = 350917980468767
 *   a(15) = 4239961392742933
 *   a(16) = 51247532773412135
 *   a(17) = 619304595300705203
 *   a(18) = 7484739788762129061
 *
 *  The function below uses an on‑demand cache and is correct for all n
 *  (with exact output whenever it fits in seq_t).
 * ====================================================================== */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_from_str.h"

/* --------------------------------------------------------------------- */
/*  Base values (a(0) … a(18)) – stored as strings for arbitrary‑precision */
static const char *const init_str[19] = {
    "1", "1", "55", "341", "5933",
    "59925", "795611", "9167119", "113555791", "1355115601",
    "16484061769", "198549329897", "2403674442213", "29023432116879",
    "350917980468767", "4239961392742933", "51247532773412135",
    "619304595300705203", "7484739788762129061"
};

static seq_t init_vals[19];     /* numeric versions of the strings   */
static bool init_done = false;  /* flag indicating that conversion   */

/* --------------------------------------------------------------------- */
/*  Convert the string constants to seq_t once */
static void fill_initial_values(void)
{
    if (!init_done) {
        for (idx_t i = 0; i < 19; ++i)
            init_vals[i] = seq_from_str(init_str[i]);
        init_done = true;
    }
}

/* --------------------------------------------------------------------- */
/*  Function implementing the recurrence with static memoisation   */
seq_t a(idx_t n)
{
    /* Ensure base values are available */
    fill_initial_values();

    /* Static cache for previously computed terms */
    static seq_t *cache      = NULL; /* array of computed a(k) */
    static size_t cache_cap  = 0;    /* allocated capacity (bytes) */
    static size_t cache_size = 0;    /* highest index + 1 currently filled */

    /* First call: initialise the cache */
    if (cache == NULL) {
        cache_cap = (size_t)64;                /* initial capacity */
        if (cache_cap < 19) cache_cap = 19;    /* need at least base terms */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;           /* allocation failure */

        for (size_t i = 0; i < 19; ++i)
            cache[i] = init_vals[i];          /* store the base values   */
        cache_size = 19;                      /* filled indices [0,18] */
    }

    /* Ensure the cache is large enough for n */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        if (new_cap < cache_cap * 2) new_cap = cache_cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Return if already computed */
    if ((size_t)n < cache_size) return cache[n];

    /* Compute terms from cache_size up to n using the recurrence */
    for (idx_t k = cache_size; k <= n; ++k) {
        /* Positive part of the recurrence */
        seq_t pos = (seq_t)6     * cache[k-1];
        pos += (seq_t)110   * cache[k-2];
        pos += (seq_t)5916  * cache[k-5];
        pos += (seq_t)25168 * cache[k-6];
        pos += (seq_t)197820* cache[k-9];
        pos += (seq_t)193866* cache[k-10];
        pos += (seq_t)279636* cache[k-13];
        pos += (seq_t)137864* cache[k-14];
        pos += (seq_t)20214 * cache[k-17];
        pos += (seq_t)2460  * cache[k-18];

        /* Negative part of the recurrence */
        seq_t neg = (seq_t)262   * cache[k-3];
        neg += (seq_t)2786    * cache[k-4];
        neg += (seq_t)53907   * cache[k-7];
        neg += (seq_t)95299   * cache[k-8];
        neg += (seq_t)340168  * cache[k-11];
        neg += (seq_t)228528  * cache[k-12];
        neg += (seq_t)108909  * cache[k-15];
        neg += (seq_t)33736   * cache[k-16];
        neg += (seq_t)1296    * cache[k-19];

        /* Final value: positive part minus negative part */
        cache[k] = pos - neg;
    }

    cache_size = (size_t)n + 1;   /* we now have all indices < n+1 filled */
    return cache[n];
}
