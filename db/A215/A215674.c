/*
 * A215674  a(1) = 1, a(n) = 2 if 1<n<=3, a(3n) = a(n)+1, a(3n+1) = a(3n+2) =
 *          a(n)+a(n+1)+1 otherwise.
 * https://oeis.org/A215674
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1; a(2)=a(3)=2; For n>3: if n%3==0: a(n)=a(n/3)+1 else :
 *            a(n)=a(n/3)+a(n/3+1)+1 (i.e. a(3n)=a(n)+1;
 *            a(3n+1)=a(3n+2)=a(n)+a(n+1)+1)
 * Range:     u8 n <= 3279, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;   /* cache[n] stores a(n), 0 indicates unset */
static idx_t cache_size = 0;  /* current allocated size */

static void ensure_cache(idx_t n)
{
    if (n + 1 <= cache_size) return;

    idx_t new_size = cache_size ? cache_size : 16;
    while (new_size < n + 1) new_size <<= 1;          /* double until big enough */

    seq_t *tmp = (seq_t *)realloc(cache, (new_size + 1) * sizeof(seq_t));
    if (!tmp) abort();                                 /* malloc failure */
    /* initialise new entries to 0 */
    for (idx_t i = cache_size + 1; i <= new_size; ++i) tmp[i] = 0;

    cache = tmp;
    cache_size = new_size;
}

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n <= 3) return (seq_t)2;

    ensure_cache(n);
    if (cache[n] != 0) return cache[n];

    idx_t div = n / 3;
    idx_t mod = n % 3;
    seq_t res;
    if (mod == 0)
        res = a(div) + (seq_t)1;
    else
        res = a(div) + a(div + 1) + (seq_t)1;

    cache[n] = res;
    return res;
}
