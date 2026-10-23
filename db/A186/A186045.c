/*
 * A186045  Number of (n+2)X4 binary arrays with each 3X3 subblock singular.
 * https://oeis.org/A186045
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            14*a(n-1)-45*a(n-2)+184*a(n-3)-1468*a(n-4)+3604*a(n-5)-10460*a(n-6)+45192*a(n-7)-66568*a(n-8)+121728*a(n-9)-342032*a(n-10)+234240*a(n-11)-343296*a(n-12)+580608*a(n-13)
 *            with
 *            a(1)=2038,a(2)=21466,a(3)=224698,a(4)=2270854,a(5)=23369314,a(6)=240280270,a(7)=2466606274,a(8)=25348415950,a(9)=2604...
 * Range:     u8 none, u16 n <= 2, u32 n <= 7, u64 n <= 16, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static const seq_t init[13] = {
    (seq_t)2038ULL,
    (seq_t)21466ULL,
    (seq_t)224698ULL,
    (seq_t)2270854ULL,
    (seq_t)23369314ULL,
    (seq_t)240280270ULL,
    (seq_t)2466606274ULL,
    (seq_t)25348415950ULL,
    (seq_t)260479732738ULL,
    (seq_t)2676521643406ULL,
    (seq_t)27504010370530ULL,
    (seq_t)282631133426158ULL,
    (seq_t)2904313272748546ULL
};

static seq_t *cache = NULL;       /* 1‑based array of computed terms */
static idx_t cache_len = 0;       /* highest n that has been computed */

/* ensure that cache has terms up to n computed */
static void ensure_cache(idx_t n)
{
    if (n <= cache_len) return;

    size_t new_len = (size_t)n + 1;   /* 1‑based array */

    if (cache == NULL) {
        cache = (seq_t *)malloc(new_len * sizeof(seq_t));
        if (!cache) exit(1);
    } else if (new_len > (size_t)cache_len + 1) {
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
    }

    for (idx_t i = cache_len + 1; i <= n; ++i) {
        if (i <= 13) {
            cache[i] = init[i - 1];
        } else {
            seq_t sum = 0;
            /* positive coefficients first to keep sum non‑negative */
            sum += (seq_t)14   * cache[i - 1];
            sum += (seq_t)184  * cache[i - 3];
            sum += (seq_t)3604 * cache[i - 5];
            sum += (seq_t)45192 * cache[i - 7];
            sum += (seq_t)121728 * cache[i - 9];
            sum += (seq_t)234240 * cache[i - 11];
            sum += (seq_t)580608 * cache[i - 13];
            /* subtract negative contributions */
            sum -= (seq_t)45     * cache[i - 2];
            sum -= (seq_t)1468   * cache[i - 4];
            sum -= (seq_t)10460  * cache[i - 6];
            sum -= (seq_t)66568  * cache[i - 8];
            sum -= (seq_t)342032 * cache[i - 10];
            sum -= (seq_t)343296 * cache[i - 12];
            cache[i] = sum;
        }
    }
    cache_len = n;
}

/* external function required by the test harness */
seq_t a(idx_t n)
{
    if (n < 1) return 0;
    ensure_cache(n);
    return cache[n];
}
