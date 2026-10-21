/*
 * A229682  Number of defective 3-colorings of an n X 5 0..2 array connected
 *          diagonally and antidiagonally with exactly two mistakes, and
 *          colors introduced in row-major 0..2 order.
 * https://oeis.org/A229682
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 45*a(n-1) - 675*a(n-2) + 2565*a(n-3) + 25272*a(n-4) -
 *            211410*a(n-5) + 3805380*a(n-7) - 8188128*a(n-8) -
 *            14959080*a(n-9) + 70858800*a(n-10) - 85030560*a(n-11) +
 *            34012224*a(n-12) for n > 13 Initial values: a(1)=0, a(2)=2688,
 *            a(3)=108000, a(4)=2700432, a(5)=58038768, a(6)=1138164048,
 *            a(7)=210...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 14, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_terms[13] = {
    (seq_t)0ULL,
    (seq_t)2688ULL,
    (seq_t)108000ULL,
    (seq_t)2700432ULL,
    (seq_t)58038768ULL,
    (seq_t)1138164048ULL,
    (seq_t)21063718224ULL,
    (seq_t)373936700880ULL,
    (seq_t)6435143958672ULL,
    (seq_t)108084508966224ULL,
    (seq_t)1780281966880656ULL,
    (seq_t)28856162624878800ULL,
    (seq_t)461471700766361616ULL
};

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_len = 0;

static void ensure_capacity(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n <= cache_len) return cache[n - 1];

    ensure_capacity((size_t)n);

    for (idx_t i = cache_len; i < n; ++i) {
        seq_t val;
        if ((idx_t)(i + 1) <= 13) {
            val = init_terms[i];
        } else {
            val = (seq_t)0;
            val += (seq_t)45     * cache[(size_t)(i - 1)];
            val -= (seq_t)675    * cache[(size_t)(i - 2)];
            val += (seq_t)2565   * cache[(size_t)(i - 3)];
            val += (seq_t)25272  * cache[(size_t)(i - 4)];
            val -= (seq_t)211410 * cache[(size_t)(i - 5)];
            val += (seq_t)3805380 * cache[(size_t)(i - 7)];
            val -= (seq_t)8188128 * cache[(size_t)(i - 8)];
            val -= (seq_t)14959080* cache[(size_t)(i - 9)];
            val += (seq_t)70858800* cache[(size_t)(i - 10)];
            val -= (seq_t)85030560* cache[(size_t)(i - 11)];
            val += (seq_t)34012224* cache[(size_t)(i - 12)];
        }
        cache[(size_t)i] = val;
    }
    cache_len = n;
    return cache[(size_t)(n - 1)];
}
