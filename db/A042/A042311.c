/*
 * A042311  Denominators of continued fraction convergents to sqrt(682).
 * https://oeis.org/A042311
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 16, u64 n <= 30, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 2395802*a(n-10) - a(n-20) for n>19
   Initial values: a(0)=1, a(1)=8, a(2)=9, a(3)=26, a(4)=139,
   a(5)=304, a(6)=1659, a(7)=3622, a(8)=5281, a(9)=45870,
   a(10)=2390521, a(11)=19170038, a(12)=21560559, a(13)=62291156,
   a(14)=333016339, a(15)=728323834, a(16)=3974635509,
   a(17)=8677594852, a(18)=12652230361, a(19)=109895437740
*/
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;

static const seq_t init_vals[20] = {
    (seq_t)1ULL, (seq_t)8ULL, (seq_t)9ULL, (seq_t)26ULL, (seq_t)139ULL,
    (seq_t)304ULL, (seq_t)1659ULL, (seq_t)3622ULL, (seq_t)5281ULL,
    (seq_t)45870ULL, (seq_t)2390521ULL, (seq_t)19170038ULL,
    (seq_t)21560559ULL, (seq_t)62291156ULL, (seq_t)333016339ULL,
    (seq_t)728323834ULL, (seq_t)3974635509ULL, (seq_t)8677594852ULL,
    (seq_t)12652230361ULL, (seq_t)109895437740ULL
};

static void ensure_cache(idx_t need)
{
    if (cache && cache_size > need)
        return;
    size_t new_len = (size_t)(need + 1);
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp)
        exit(1);
    if (new_len > cache_size) {
        for (size_t i = cache_size; i < new_len; ++i)
            tmp[i] = (seq_t)0;
    }
    cache = tmp;
    cache_size = new_len;
}

seq_t a(idx_t n)
{
    ensure_cache(n);

    /* Initialize first 20 terms on each call; harmless and always correct. */
    for (idx_t i = 0; i < 20 && i <= n; ++i)
        cache[i] = init_vals[i];

    if (n < 20)
        return cache[n];

    const seq_t coeff = (seq_t)2395802;

    for (idx_t k = 20; k <= n; ++k) {
        if (cache[k] == (seq_t)0) {
            cache[k] = coeff * cache[k - 10] - cache[k - 20];
        }
    }

    return cache[n];
}
