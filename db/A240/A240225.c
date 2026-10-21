/*
 * A240225  Total number of parts of the partitions of n into distinct
 *          Fibonacci numbers.
 * https://oeis.org/A240225
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 1298, u16 n <= 9976, u32 n <= 9976, u64 n <= 9976,
 *            u128 n <= 9976, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A240225 – total number of parts of the partitions of n
 *  into distinct Fibonacci numbers (F(2)=1,F(3)=2,F(4)=3,…).
 *
 *  Implementation: dynamic programming over the distinct Fibonacci
 *  numbers.  For each Fibonacci part f we update two tables from
 *  high to low to enforce the “distinct” condition.  Let
 *
 *      cnt[s]   = number of subsets of distinct Fibonacci numbers that
 *                 sum to s,
 *
 *      parts[s] = total number of parts in all those subsets.
 *
 *  When a new part f is considered, every previously counted subset
 *  summing to (s–f) gives rise to a new subset summing to s with
 *  one additional part.  Hence
 *
 *      cnt[s]   += cnt[s‑f];
 *      parts[s] += parts[s‑f] + cnt[s‑f];
 *
 *  After processing all Fibonacci numbers ≤ n, the answer is parts[n].
 *
 *  This algorithm runs in O(n · F(n)) time where F(n) is the number of
 *  Fibonacci numbers ≤ n (≈ log φ · n).
 */

#include <stdlib.h>   /* size_t, calloc, free */

seq_t a(idx_t n)
{
    /* n = 0 has the empty partition which contributes 0 parts. */
    if (n == (idx_t)0) {
        return (seq_t)0;
    }

    /* Allocate arrays of size n+1.  Use calloc so they are zeroed. */
    size_t size = (size_t)n + 1;
    seq_t *cnt   = (seq_t *)calloc(size, sizeof(seq_t));
    seq_t *parts = (seq_t *)calloc(size, sizeof(seq_t));

    if (!cnt || !parts) {
        /* allocation failed – clean up and return 0 */
        free(cnt);
        free(parts);
        return (seq_t)0;
    }

    /* Base case: one way (the empty set) to sum to 0 with 0 parts. */
    cnt[0]   = (seq_t)1;
    parts[0] = (seq_t)0;

    /* Generate Fibonacci numbers f₀=1 (F₂), f₁=2 (F₃). */
    idx_t f0 = 1;
    idx_t f1 = 2;
    while (f0 <= n) {
        idx_t f = f0;

        /* Update from high to low to avoid reuse of f in the same pass. */
        for (idx_t s = n; ; --s) {
            if (s < f) {
                break;
            }
            seq_t cnt_prev   = cnt[s - f];
            seq_t parts_prev = parts[s - f];

            cnt[s]   += cnt_prev;
            parts[s] += parts_prev + cnt_prev;   /* new part contributes +1 */
        }

        /* Move to the next Fibonacci number. */
        idx_t next_f = f0 + f1;
        f0 = f1;
        f1 = next_f;
    }

    seq_t result = parts[n];

    free(cnt);
    free(parts);
    return result;
}
