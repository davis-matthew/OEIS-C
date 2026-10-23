/*
 * A036354  Heptagonal square numbers.
 * https://oeis.org/A036354
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2079362*a(n-3) - a(n-6) + 233928 for n≥7 Initial values:
 *            a(1)=1, a(2)=81, a(3)=5929, a(4)=2307361, a(5)=168662169,
 *            a(6)=12328771225
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Store the first six terms (index 0 is unused) */
    static const seq_t init[7] = {
        0,
        1,
        81,
        5929,
        2307361,
        168662169,
        (seq_t)12328771225ULL
    };

    static seq_t *seq = NULL;
    static size_t last = 0;  /* highest computed index (1‑based) */
    static size_t cap  = 0;  /* current allocation size */

    if (n <= 6) return init[n];

    /* Lazy allocation and initialization of the cache */
    if (seq == NULL) {
        cap = 64;  /* initial capacity (more than enough for n=6) */
        seq = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!seq) exit(1);  /* simple error handling */
        for (size_t i = 1; i <= 6; ++i)
            seq[i] = init[i];
        last = 6;
    }

    /* Grow the array if the requested index exceeds current capacity */
    if ((size_t)n >= cap) {
        size_t new_cap = cap;
        while (new_cap <= (size_t)n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        seq = tmp;
        cap = new_cap;
    }

    /* Constants for the linear recurrence */
    const seq_t C1 = (seq_t)2079362;
    const seq_t C2 = (seq_t)233928;

    /* Compute terms up to the requested index using the recurrence:
       a(i) = 2079362*a(i-3) - a(i-6) + 233928 */
    for (size_t i = last + 1; i <= (size_t)n; ++i) {
        seq[i] = C1 * seq[i - 3] - seq[i - 6] + C2;
    }
    last = (size_t)n;
    return seq[n];
}
