/*
 * A227376  G.f.: 1/(1 - x - x^2 - x^3 + x^5 + x^6 + x^7).
 * https://oeis.org/A227376
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + a(n-3) - a(n-5) - a(n-6) - a(n-7);
 *            Init: a(0)=1,a(1)=1,a(2)=2,a(3)=4,a(4)=7,a(5)=12,a(6)=21;
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *a_cache = NULL;
static size_t a_cache_cap = 0;
static idx_t a_cache_len = 0;

static void ensure_a_cache(idx_t n)
{
    if (n < a_cache_len)
        return;
    size_t needed = (size_t)(n + 1);
    if (needed <= a_cache_cap)
        return;
    size_t new_cap = a_cache_cap ? a_cache_cap << 1 : 16;
    if (new_cap < needed)
        new_cap = needed;
    seq_t *new_mem = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    a_cache = new_mem;
    a_cache_cap = new_cap;
}

static void init_a_cache(void)
{
    if (a_cache_len)
        return;
    ensure_a_cache(6);
    a_cache[0] = (seq_t)1;
    a_cache[1] = (seq_t)1;
    a_cache[2] = (seq_t)2;
    a_cache[3] = (seq_t)4;
    a_cache[4] = (seq_t)7;
    a_cache[5] = (seq_t)12;
    a_cache[6] = (seq_t)21;
    a_cache_len = 7;
}

seq_t a(idx_t n)
{
    init_a_cache();
    ensure_a_cache(n);
    if (n < a_cache_len)
        return a_cache[n];
    for (idx_t i = a_cache_len; i <= n; ++i) {
        seq_t val = a_cache[i-1] + a_cache[i-2] + a_cache[i-3];
        val -= a_cache[i-5];
        val -= a_cache[i-6];
        val -= a_cache[i-7];
        a_cache[i] = val;
    }
    a_cache_len = n + 1;
    return a_cache[n];
}
