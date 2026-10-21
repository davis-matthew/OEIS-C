/*
 * A063298  Dimension of the space of weight n cuspidal newforms for Gamma_1(
 *          25 ).
 * https://oeis.org/A063298
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = a(n-4) + a(n-6) - a(n-10) for n>13. Base values: a(2)=-1,
 *            a(3)=12, a(4)=36, a(5)=59, a(6)=82, a(7)=105, a(8)=128,
 *            a(9)=151, a(10)=174, a(11)=197, a(12)=220, a(13)=244
 * Range:     i8 n <= 7, i16 n <= 49, i32 n <= 49, i64 n <= 49, i128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Static storage for memoisation */
static seq_t *seq_cache   = NULL;
static idx_t seq_capacity = 0;
static idx_t seq_last     = 1; /* highest computed index (>=1) */

/* Initialise cache with base values up to index 13 */
static void seq_init_cache(void)
{
    if (seq_cache != NULL)
        return;                    /* already initialised */

    seq_capacity = 14;             /* indices 0..13 */
    seq_cache = (seq_t *)malloc(sizeof(seq_t) * seq_capacity);
    for (idx_t i = 0; i < seq_capacity; ++i)
        seq_cache[i] = (seq_t)0;   /* clear */

    /* base values */
    seq_cache[2]  = (seq_t)-1;
    seq_cache[3]  = (seq_t)12;
    seq_cache[4]  = (seq_t)36;
    seq_cache[5]  = (seq_t)59;
    seq_cache[6]  = (seq_t)82;
    seq_cache[7]  = (seq_t)105;
    seq_cache[8]  = (seq_t)128;
    seq_cache[9]  = (seq_t)151;
    seq_cache[10] = (seq_t)174;
    seq_cache[11] = (seq_t)197;
    seq_cache[12] = (seq_t)220;
    seq_cache[13] = (seq_t)244;

    seq_last = 13;
}

/* Grow the cache to at least 'needed' elements */
static void seq_grow_capacity(idx_t needed)
{
    if (seq_capacity >= needed)
        return;

    idx_t new_cap = needed > seq_capacity * 2 ? needed : seq_capacity * 2;
    seq_cache = (seq_t *)realloc(seq_cache, sizeof(seq_t) * new_cap);
    for (idx_t i = seq_capacity; i < new_cap; ++i)
        seq_cache[i] = (seq_t)0;
    seq_capacity = new_cap;
}

/* Public function computing the sequence term */
seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;          /* undefined for n < 2 */

    seq_init_cache();

    if (n >= seq_capacity)
        seq_grow_capacity(n + 1);

    if (n > seq_last) {
        for (idx_t i = seq_last + 1; i <= n; ++i) {
            seq_t val = seq_cache[i - 4] + seq_cache[i - 6] - seq_cache[i - 10];
            seq_cache[i] = val;
        }
        seq_last = n;
    }

    return seq_cache[n];
}
