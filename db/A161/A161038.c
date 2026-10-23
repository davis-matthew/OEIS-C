/*
 * A161038  Number of partitions of n into Fibonacci numbers where every part
 *          appears at least 14 times.
 * https://oeis.org/A161038
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Number of partitions of n into Fibonacci numbers where every
 *            part used appears at least 14 times
 * Range:     u8 n <= 123, u16 n <= 329, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;         /* undefined for n < 1, safe return */

    /* Convert n to a size_t for array indexing (n ≤ 10000 in tests). */
    size_t N = (size_t)n;

    /* Allocate three working arrays. */
    seq_t *curr = alloc_seq_array(N + 1);
    seq_t *next = alloc_seq_array(N + 1);
    seq_t *pref = alloc_seq_array(N + 1);
    if (!curr || !next || !pref) {
        free_seq_array(curr); free_seq_array(next); free_seq_array(pref);
        return (seq_t)0;
    }

    /* initialise */
    for (size_t i = 0; i <= N; ++i) curr[i] = 0;
    curr[0] = 1;

    /* Generate Fibonacci numbers up to N (first two: 1, 2). */
    uint64_t fibs[30];
    int fib_cnt = 0;
    uint64_t f0 = 1, f1 = 2;
    if (f0 <= N) fibs[fib_cnt++] = f0;
    if (f1 <= N) fibs[fib_cnt++] = f1;
    while (1) {
        uint64_t f2 = f0 + f1;
        if (f2 > N) break;
        fibs[fib_cnt++] = f2;
        f0 = f1; f1 = f2;
    }

    /* DP: for each Fibonacci part, account for either 0 copies or
       at least 14 copies (and then any further copies). */
    for (int fi = 0; fi < fib_cnt; ++fi) {
        uint64_t f = fibs[fi];          /* 1, 2, 3, 5, ...          */
        /* Process each residue modulo f. */
        for (uint64_t r = 0; r < f; ++r) {
            /* Build prefix sums of curr over this residue class. */
            size_t m = 0;
            for (uint64_t j = r; j <= N; j += f, ++m) {
                pref[m] = curr[j] + (m > 0 ? pref[m - 1] : (seq_t)0);
            }

            /* Compute new values for this residue. */
            m = 0;
            for (uint64_t j = r; j <= N; j += f, ++m) {
                next[j] = curr[j];
                if (m >= 14) next[j] += pref[m - 14];
            }
        }
        /* Swap buffers for next iteration. */
        seq_t *tmp = curr; curr = next; next = tmp;
    }

    seq_t result = curr[N];
    free_seq_array(curr);
    free_seq_array(next);
    free_seq_array(pref);
    return result;
}
