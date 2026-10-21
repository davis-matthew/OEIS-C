/*
 * A221859  Expansion of (1-3*x+x^2)*(1-2*x)/(1-9*x+28*x^2-35*x^3+15*x^4-x^5).
 * https://oeis.org/A221859
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*a(n-1) - 28*a(n-2) + 35*a(n-3) - 15*a(n-4) + a(n-5)
 *            with a(0) = 1, a(1) = 4, a(2) = 15, a(3) = 56, a(4) = 209
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 33, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_size = 0;

static void ensure_capacity(idx_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < (size_t)need)
        new_cap *= 2;
    seq_t *newp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!newp)
        exit(1);
    cache = newp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < cache_size)
        return cache[n];

    ensure_capacity(n + 1);

    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t val;
        if (i == 0)      val = 1;
        else if (i == 1) val = 4;
        else if (i == 2) val = 15;
        else if (i == 3) val = 56;
        else if (i == 4) val = 209;
        else {
            val = (seq_t)9  * cache[i - 1];
            val -= (seq_t)28 * cache[i - 2];
            val += (seq_t)35 * cache[i - 3];
            val -= (seq_t)15 * cache[i - 4];
            val +=          cache[i - 5];
        }
        cache[i] = val;
    }

    cache_size = n + 1;
    return cache[n];
}
