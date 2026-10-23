/*
 * A041029  Denominators of continued fraction convergents to sqrt(19).
 * https://oeis.org/A041029
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 46, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A041029 – Denominators of continued fraction convergents to sqrt(19)
   Recurrence: a(n) = 340*a(n-6) – a(n-12)
   Initial values: a(0)=1, a(1)=2, a(2)=3, a(3)=11, a(4)=14, a(5)=39,
                   a(6)=326, a(7)=691, a(8)=1017, a(9)=3742,
                   a(10)=4759, a(11)=13260 */
#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;      /* cache of computed terms */
static size_t cache_size = 0;    /* allocated size of cache */
static size_t computed = 0;      /* index after which cache is valid */

/* Prefilled first 12 terms */
static const seq_t init_terms[12] = {
    (seq_t)1,  (seq_t)2,  (seq_t)3,  (seq_t)11,
    (seq_t)14, (seq_t)39, (seq_t)326, (seq_t)691,
    (seq_t)1017, (seq_t)3742, (seq_t)4759, (seq_t)13260
};

seq_t a(idx_t n)
{
    /* Allocate or grow cache to hold index n */
    if (cache == NULL) {
        cache_size = (size_t)n + 1;
        cache = (seq_t *)calloc(cache_size, sizeof(seq_t));
        computed = 0;
    } else if (cache_size <= (size_t)n) {
        size_t new_size = cache_size ? cache_size : 1;
        while (new_size <= (size_t)n) new_size <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        cache = tmp;
        cache_size = new_size;
    }

    /* Compute terms up to n, if necessary */
    for (size_t i = computed; i <= (size_t)n; ++i) {
        if (i < 12) {
            cache[i] = init_terms[(size_t)i];
        } else {
            cache[i] = (seq_t)340 * cache[i - 6] - cache[i - 12];
        }
    }
    computed = (size_t)n + 1;

    return cache[(size_t)n];
}
