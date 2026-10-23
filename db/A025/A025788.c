/*
 * A025788  Expansion of 1/((1-x)(1-x^7)(1-x^12)).
 * https://oeis.org/A025788
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)+a(n-7)-a(n-8)+a(n-12)-a(n-13)-a(n-19)+a(n-20)
 *            Initial values: a(0)=1, a(1)=1, a(2)=1, a(3)=1, a(4)=1, a(5)=1,
 *            a(6)=1, a(7)=2, a(8)=2, a(9)=2, a(10)=2, a(11)=2, a(12)=3,
 *            a(13)=3, a(14)=4, a(15)=4, a(16)=4, a(17)=4, a(18)=4, a(19)=5,
 *            a(20)=5, a(21)=6,
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;
static size_t cache_cap = 0;

static void ensure_capacity(idx_t need);

seq_t a(idx_t n)
{
    if (n >= cache_cap) {
        ensure_capacity(n + 1);
    }
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)1;
            continue;
        }
        seq_t val = cache[i-1];
        if (i >= 7)  val += cache[i-7];
        if (i >= 8)  val -= cache[i-8];
        if (i >= 12) val += cache[i-12];
        if (i >= 13) val -= cache[i-13];
        if (i >= 19) val -= cache[i-19];
        if (i >= 20) val += cache[i-20];
        cache[i] = val;
    }
    cache_len = n + 1;
    return cache[n];
}

static void ensure_capacity(idx_t need)
{
    size_t new_cap = cache_cap > 0 ? cache_cap : 16;
    while (new_cap < (size_t)need) {
        new_cap <<= 1;
    }
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (tmp == NULL) abort();
    cache = tmp;
    cache_cap = new_cap;
}
