/*
 * A041463  Denominators of continued fraction convergents to sqrt(247).
 * https://oeis.org/A041463
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 170584*a(n-12) - a(n-24) for n > 23 with initial values
 *            a(0)=1, a(1)=1, a(2)=3, a(3)=4, a(4)=7, a(5)=67, a(6)=74,
 *            a(7)=733, a(8)=807, a(9)=1540, a(10)=3887, a(11)=5427,
 *            a(12)=166697, a(13)=172124, a(14)=510945, a(15)=683069,
 *            a(16)=1194014, a(17)=11429195, a(18)=12623209, a(19)=125038076,
 *            a...
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 23, u64 n <= 45, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_terms[24] = {
    (seq_t)1,  (seq_t)1,  (seq_t)3,  (seq_t)4,  (seq_t)7,
    (seq_t)67, (seq_t)74, (seq_t)733, (seq_t)807, (seq_t)1540,
    (seq_t)3887, (seq_t)5427, (seq_t)166697, (seq_t)172124,
    (seq_t)510945, (seq_t)683069, (seq_t)1194014,
    (seq_t)11429195, (seq_t)12623209, (seq_t)125038076,
    (seq_t)137661285, (seq_t)262699361, (seq_t)663060007,
    (seq_t)925759368
};

static seq_t *cache = NULL;
static size_t cache_size = 0;     /* number of terms currently computed */
static size_t cache_capacity = 0; /* allocated capacity */

seq_t a(idx_t n)
{
    /* If n is within the first 24 terms, return the hard‑coded value. */
    if (n < 24)
        return init_terms[(size_t)n];

    /* Ensure cache is initialized with the first 24 terms. */
    if (cache == NULL) {
        size_t cap = 24;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL)
            abort();
        for (size_t i = 0; i < 24; ++i)
            cache[i] = init_terms[i];
        cache_size = 24;
        cache_capacity = cap;
    }

    size_t up_to = (size_t)n;

    /* Expand storage if necessary. */
    if (up_to + 1 > cache_capacity) {
        size_t new_cap = cache_capacity ? cache_capacity * 2 : 24;
        while (new_cap <= up_to) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL)
            abort();
        cache = tmp;
        cache_capacity = new_cap;
    }

    /* Compute missing terms using the recurrence. */
    if (up_to + 1 > cache_size) {
        for (size_t i = cache_size; i <= up_to; ++i) {
            cache[i] = (seq_t)170584 * cache[i - 12] - cache[i - 24];
        }
        cache_size = up_to + 1;
    }

    return cache[up_to];
}
