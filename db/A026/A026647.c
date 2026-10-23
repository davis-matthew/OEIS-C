/*
 * A026647  a(n) = Sum_{k=0..floor(n/2)} A026637(n-k, k).
 * https://oeis.org/A026647
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2) + a(n-3) + 2*a(n-4) + a(n-5) + 3 with a(0)=1,
 *            a(1)=1, a(2)=2, a(3)=3, a(4)=6, a(5)=10
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 46, u64 n <= 92, u128 n <= 184,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t computed_len = 0;   /* number of entries already stored */

static void ensure_capacity(idx_t n)
{
    size_t need = (size_t)(n + 1);
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < need)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp && new_cap > 0)
        abort();                     /* allocation failure */
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    ensure_capacity(n);
    size_t i;
    for (i = computed_len; i <= (size_t)n; ++i) {
        seq_t val;
        switch (i) {
        case 0: val = 1; break;
        case 1: val = 1; break;
        case 2: val = 2; break;
        case 3: val = 3; break;
        case 4: val = 6; break;
        case 5: val = 10; break;
        default:
            val = cache[i - 2]
                + cache[i - 3]
                + cache[i - 4] * (seq_t)2
                + cache[i - 5]
                + (seq_t)3;
            break;
        }
        cache[i] = val;
    }
    computed_len = (size_t)n + 1;
    return cache[(size_t)n];
}
