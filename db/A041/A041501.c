/*
 * A041501  Denominators of continued fraction convergents to sqrt(267).
 * https://oeis.org/A041501
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 17, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(0)=1, a(1)=2, a(2)=3, a(3)=47, a(4)=50, a(5)=147,
   a(6)=4754, a(7)=9655, a(8)=14409, a(9)=225790,
   a(10)=240199, a(11)=706188.
   For n>11:  a(n) = 4804*a(n-6) - a(n-12)   (integer arithmetic) */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t  cache_size = 0;   /* number of entries allocated */
static idx_t  last_computed = 0;/* largest n whose a(n) is valid */

/* Ensure that cache[0 .. n] is allocated.  No reallocation if already sufficient. */
static void ensure_cache_size(idx_t n)
{
    if (cache_size <= n)
    {
        idx_t new_size = n + 1;
        cache = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        cache_size = new_size;
    }
}

/* Initialise the known base cases for n = 0 .. 11.  This is done once. */
static void init_base(void)
{
    if (last_computed >= 11) return;
    ensure_cache_size(12);
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)2;
    cache[2] = (seq_t)3;
    cache[3] = (seq_t)47;
    cache[4] = (seq_t)50;
    cache[5] = (seq_t)147;
    cache[6] = (seq_t)4754;
    cache[7] = (seq_t)9655;
    cache[8] = (seq_t)14409;
    cache[9] = (seq_t)225790;
    cache[10] = (seq_t)240199;
    cache[11] = (seq_t)706188;
    last_computed = 11;
}

/* Return a(n) for any n >= 0, without assuming order of calls. */
seq_t a(idx_t n)
{
    init_base();
    if (n <= 11) return cache[n];

    ensure_cache_size(n);
    if (n <= last_computed) return cache[n];

    for (idx_t i = last_computed + 1; i <= n; ++i)
    {
        /* a(i) = 4804*a(i-6) - a(i-12) */
        seq_t prod = cache[i - 6] * (seq_t)4804;
        cache[i] = prod - cache[i - 12];
    }
    last_computed = n;
    return cache[n];
}
