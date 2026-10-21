/*
 * A221366  The simple continued fraction expansion of F(x) := Product_{n >=
 *          0} (1 - x^(4*n+3))/(1 - x^(4*n+1)) when x = (1/2)*(7 - 3*sqrt(5)).
 * https://oeis.org/A221366
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, cofr
 * Formula:   a(0)=1, a(1)=5, a(2)=1, a(3)=45, a(4)=1, a(5)=320,
 *            a(n)=8*a(n-2)-8*a(n-4)+a(n-6) for n>=6
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 22, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;      /* array of computed terms */
static size_t cache_len = 0;     /* current capacity */

/* ensure that cache contains at least terms up to index n */
static void ensure_cache(idx_t n)
{
    if (cache_len > n) return;              /* already sufficient */

    size_t old_len   = cache_len;
    size_t new_len   = n + 1;                /* need indices 0..n */
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) abort();                       /* out of memory */
    cache = tmp;

    for (idx_t i = old_len; i <= n; ++i) {
        seq_t val;
        if (i == 0)        val = (seq_t)1;
        else if (i == 1)   val = (seq_t)5;
        else if (i == 2)   val = (seq_t)1;
        else if (i == 3)   val = (seq_t)45;
        else if (i == 4)   val = (seq_t)1;
        else if (i == 5)   val = (seq_t)320;
        else {
            seq_t t2 = cache[i-2];
            seq_t t4 = cache[i-4];
            seq_t t6 = cache[i-6];
            val = (seq_t)8 * t2 - (seq_t)8 * t4 + t6;
        }
        cache[i] = val;
    }
    cache_len = new_len;
}

/* external function requested by the build system */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
