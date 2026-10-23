/*
 * A121530  Number of double rises at an odd level in all nondecreasing Dyck
 *          paths of semilength n. A nondecreasing Dyck path is a Dyck path
 *          for which the sequence of the altitudes of the valleys is
 *          nondecreasing.
 * https://oeis.org/A121530
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) satisfies the linear recurrence a(n) = 6*a(n-1) - 9*a(n-2)
 *            - 5*a(n-3) + 15*a(n-4) - a(n-5) - 4*a(n-6) + a(n-7) for n > 7
 *            Initial values: a(1)=0, a(2)=1, a(3)=4, a(4)=14, a(5)=47,
 *            a(6)=148, a(7)=454
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;          /* array of computed terms, index 0 unused */
    static size_t cache_capacity = 0;    /* total allocation size (elements)   */
    static size_t cache_len = 0;         /* number of elements actually filled  */

    /* initial caching of the first 7 terms */
    if (cache == NULL) {
        cache_capacity = 8;              /* allocate space for indices 0..7  */
        cache = (seq_t *)malloc(cache_capacity * sizeof(seq_t));
        cache_len = 8;                   /* indices 0..7 are valid          */
        cache[0] = (seq_t)0;             /* unused but harmless             */
        cache[1] = (seq_t)0;             /* a(1) */
        cache[2] = (seq_t)1;             /* a(2) */
        cache[3] = (seq_t)4;             /* a(3) */
        cache[4] = (seq_t)14;            /* a(4) */
        cache[5] = (seq_t)47;            /* a(5) */
        cache[6] = (seq_t)148;           /* a(6) */
        cache[7] = (seq_t)454;           /* a(7) */
    }

    /* return if already computed */
    if ((size_t)n < cache_len)
        return cache[(size_t)n];

    /* enlarge allocation if necessary */
    size_t required = (size_t)n + 1;
    if (required > cache_capacity) {
        size_t new_cap = cache_capacity ? cache_capacity : 8;
        while (new_cap < required) new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_capacity = new_cap;
    }

    /* compute missing terms up to n using the recurrence */
    for (size_t i = cache_len; i < required; ++i) {
        seq_t sum_pos =
            6 * cache[i-1] + 15 * cache[i-4] + cache[i-7];
        seq_t sum_neg =
            9 * cache[i-2] + 5 * cache[i-3] + cache[i-5] + 4 * cache[i-6];
        cache[i] = sum_pos - sum_neg;  /* result is non‑negative */
    }
    cache_len = required;
    return cache[(size_t)n];
}
