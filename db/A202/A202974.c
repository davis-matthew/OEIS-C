/*
 * A202974  Number of n X 3 0..1 arrays with every nonzero element less than
 *          or equal to at least two horizontal and vertical neighbors.
 * https://oeis.org/A202974
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - 11*a(n-2) + 16*a(n-3) - 14*a(n-4) - 3*a(n-5) +
 *            8*a(n-6) - 9*a(n-7) - 2*a(n-8) + 6*a(n-9) + 2*a(n-10) (n ≥ 11)
 *            a(1)=1, a(2)=4, a(3)=17, a(4)=58, a(5)=215, a(6)=866, a(7)=3507,
 *            a(8)=14120, a(9)=56921, a(10)=229860
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* array of terms, cache[0] unused */
    static size_t cap = 0;        /* allocated size (i.e., max index + 1) */

    /* Small table of the first ten terms */
    static const seq_t init[11] = {
        0,                  /* unused, a(0) */
        (seq_t)1,           /* a(1) */
        (seq_t)4,           /* a(2) */
        (seq_t)17,          /* a(3) */
        (seq_t)58,          /* a(4) */
        (seq_t)215,         /* a(5) */
        (seq_t)866,         /* a(6) */
        (seq_t)3507,        /* a(7) */
        (seq_t)14120,       /* a(8) */
        (seq_t)56921,       /* a(9) */
        (seq_t)229860       /* a(10) */
    };

    /* For very small n, return directly */
    if (n <= 10)
        return init[n];

    size_t N = (size_t)n;           /* index to compute */
    size_t prev_cap = cap;           /* capacity before potential reallocation */

    /* Allocate or grow the cache if necessary */
    if (cap <= N) {
        seq_t *new_cache = (seq_t *)realloc(cache,
                                            (N + 1) * sizeof(seq_t));
        if (!new_cache) abort();    /* out‑of‑memory: terminate */
        cache = new_cache;

        /* Fill the initial values if they were not already present */
        if (prev_cap == 0) {
            for (idx_t i = 1; i <= 10 && i <= (idx_t)N; ++i)
                cache[i] = init[i];
        } else if (prev_cap <= 10) {
            /* The first 10 entries were already set earlier; nothing to do */
        }
        cap = N + 1;
    }

    /* Determine where we should start adding new values */
    size_t start;
    if (prev_cap == 0 || prev_cap <= 10)
        start = 11;            /* we already know up to a(10) */
    else
        start = prev_cap;      /* new indices appear here */

    /* Compute terms from 'start' up to N using the recurrence */
    for (size_t i = start; i <= N; ++i) {
        seq_t term = cache[i - 1] * (seq_t)6
                   - cache[i - 2] * (seq_t)11
                   + cache[i - 3] * (seq_t)16
                   - cache[i - 4] * (seq_t)14
                   - cache[i - 5] * (seq_t)3
                   + cache[i - 6] * (seq_t)8
                   - cache[i - 7] * (seq_t)9
                   - cache[i - 8] * (seq_t)2
                   + cache[i - 9] * (seq_t)6
                   + cache[i -10] * (seq_t)2;
        cache[i] = term;
    }

    return cache[N];
}
