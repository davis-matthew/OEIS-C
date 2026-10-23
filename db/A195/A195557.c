/*
 * A195557  Numerators b(n) of Pythagorean approximations b(n)/a(n) to 1/3.
 * https://oeis.org/A195557
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 37*a(n-3) + 37*a(n-6) - a(n-9) (n≥10); Initial values:
 *            a(1)=0,a(2)=5,a(3)=7,a(4)=12,a(5)=155,
 *            a(6)=301,a(7)=444,a(8)=5921,a(9)=11395
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Compute the nth term of sequence A195557 (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0)            /* undefined for n = 0 – return 0 */
        return 0;

    /* Static cache for memoisation */
    static seq_t *cache    = NULL;   /* array of computed terms */
    static idx_t  capacity = 0;      /* allocated size of array */
    static idx_t  computed = 0;      /* highest index computed so far */

    /* Ensure the cache is large enough for indices 0 … n */
    if (capacity < n + 1) {
        cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        capacity = n + 1;
    }

    /* Seed the first nine terms once, when required. */
    static const seq_t seed[10] = {
        0ULL,          /* a(0) – unused */
        0ULL,          /* a(1) */
        5ULL,          /* a(2) */
        7ULL,          /* a(3) */
        12ULL,         /* a(4) */
        155ULL,        /* a(5) */
        301ULL,        /* a(6) */
        444ULL,        /* a(7) */
        5921ULL,       /* a(8) */
        11395ULL       /* a(9) */
    };

    if (computed < 9) {
        idx_t up_to = (n < 9) ? n : 9;   /* fill only up to needed */
        for (idx_t i = computed + 1; i <= up_to; ++i)
            cache[i] = seed[i];
        computed = up_to;
    }

    /* If the requested term is already known, return it. */
    if (n <= computed)
        return cache[n];

    /* Compute missing terms using the recurrence. */
    for (idx_t i = computed + 1; i <= n; ++i) {
        cache[i] =
            (seq_t)37 * cache[i - 3] + (seq_t)37 * cache[i - 6] -
            cache[i - 9];
    }
    computed = n;
    return cache[n];
}
