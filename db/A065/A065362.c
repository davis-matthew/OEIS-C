/*
 * A065362  Rebase n from 4 to 2. Replace 4^k with 2^k in quaternary expansion
 *          of n.
 * https://oeis.org/A065362
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = 2*a(n/4) if n % 4 == 0; a(n) = a(n-1)+1 otherwise. a(1)=1
 *            (offset 1)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t   cache_size = 0;

/* Ensure cache has room for index n (0 ≤ n < cache_size). */
static void ensure_cache(idx_t n)
{
    if (n < cache_size)
        return;

    idx_t new_size = cache_size ? cache_size * 2 : 64;
    while (new_size <= n)
        new_size <<= 1;

    cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    for (idx_t i = cache_size; i < new_size; ++i)
        cache[i] = 0;  /* 0 marks “not yet computed”; seq_t ≥ 1 for n ≥ 1 */

    cache_size = new_size;
}

seq_t a(idx_t n)
{
    if (n == 0)               /* not part of the sequence, used only for safety */
        return 0;

    if (cache == NULL) {      /* first call: allocate cache and seed a(1) */
        ensure_cache(n);
        cache[1] = 1;
    } else
        ensure_cache(n);

    if (cache[n] != 0)         /* already known */
        return cache[n];

    seq_t val;
    if ((n & 3) == 0) {        /* n divisible by 4 */
        val = (seq_t)2 * a(n / 4);
    } else {                   /* else increment from predecessor */
        val = a(n - 1) + 1;
    }

    cache[n] = val;
    return val;
}
