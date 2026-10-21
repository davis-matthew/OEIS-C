/*
 * A226770  Let n+1 have proper divisors 1 < d_1,...,d_k < n+1; consider all
 *          proper divisors of n+d_1,...,n+d_k which did not appear earlier.
 *          Let them be d_{1,1}, d_{1,2},..., d_{k,1}, d_{k,2},..., d_{k,t};
 *          then consider proper divisors of n+d_{1,1},...,n+d_{k,t} which did
 *          not appear earlier, repeat until no...
 * https://oeis.org/A226770
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct proper divisors >1 obtained by the
 *            closure process described in OEIS A226770
 * Range:     u8 n <= 262, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    /* All divisors that can appear are ≤ n (see analysis). */
    size_t limit = (size_t)n + 3;

    /* Boolean array storing whether a divisor has already been seen. */
    bool *seen = (bool *)malloc(limit * sizeof(bool));
    if (!seen) return (seq_t)0;
    memset(seen, 0, limit);

    /* Simple queue for BFS over found divisors. */
    size_t *queue = (size_t *)malloc(limit * sizeof(size_t));
    if (!queue) { free(seen); return (seq_t)0; }
    size_t q_head = 0, q_tail = 0;

    /* Helper to add a new divisor if not yet seen. */
    #define ADD_DIV(d)                 \
        do {                           \
            if ((d) < limit && !seen[(d)]) { \
                seen[(d)] = true;      \
                queue[q_tail++] = (d); \
            }                          \
        } while (0)

    /* Start with proper divisors >1 of n+1. */
    uint64_t m = (uint64_t)n + 1;
    for (size_t i = 2; (uint64_t)i * i <= m; ++i) {
        if (m % i == 0) {
            size_t d1 = i, d2 = m / i;
            ADD_DIV(d1);
            if (d1 != d2) ADD_DIV(d2);
        }
    }

    /* Process the queue until no new divisors appear. */
    while (q_head < q_tail) {
        size_t d = queue[q_head++];
        uint64_t mm = (uint64_t)n + d;
        for (size_t i = 2; (uint64_t)i * i <= mm; ++i) {
            if (mm % i == 0) {
                size_t d1 = i, d2 = mm / i;
                ADD_DIV(d1);
                if (d1 != d2) ADD_DIV(d2);
            }
        }
    }

    /* Count how many divisors were seen (excluding 1). */
    size_t count = 0;
    for (size_t i = 2; i < limit; ++i)
        if (seen[i]) ++count;

    free(seen);
    free(queue);
    #undef ADD_DIV

    return (seq_t)count;
}
