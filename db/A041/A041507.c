/*
 * A041507  Denominators of continued fraction convergents to sqrt(270).
 * https://oeis.org/A041507
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 10582*a(n-6) - a(n-12) (for n > 11) initial values:
 *            a(0)=1, a(1)=2, a(2)=7, a(3)=44, a(4)=139, a(5)=322, a(6)=10443,
 *            a(7)=21208, a(8)=74067, a(9)=465610, a(10)=1470897,
 *            a(11)=3407404
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 29, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_len = 0;

/* 12 initial terms of the sequence */
static const seq_t init_vals[12] = {
    (seq_t)1,    (seq_t)2,    (seq_t)7,
    (seq_t)44,   (seq_t)139,  (seq_t)322,
    (seq_t)10443,(seq_t)21208,(seq_t)74067,
    (seq_t)465610,(seq_t)1470897,(seq_t)3407404
};

/* ensure that cache[n] is valid for all n <= requested */
static void ensure_cache(idx_t n)
{
    size_t target = (size_t)n + 1;

    if (target <= cache_len)
        return;

    /* allocate / reallocate */
    seq_t *new_cache;
    if (cache == NULL)
        new_cache = (seq_t *)malloc(target * sizeof(seq_t));
    else
        new_cache = (seq_t *)realloc(cache, target * sizeof(seq_t));

    if (!new_cache) {
        /* allocation failure – abort to avoid further errors */
        abort();
    }

    /* copy old data (realloc keeps it) and compute new entries */
    for (size_t i = cache_len; i < target; ++i) {
        if (i < 12)
            new_cache[i] = init_vals[i];
        else
            new_cache[i] = ((seq_t)10582 * new_cache[i - 6]) - new_cache[i - 12];
    }

    cache = new_cache;
    cache_len = target;
}

/* sequence entry function */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
