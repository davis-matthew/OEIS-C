/*
 * A238660  Number of partitions of n having population standard deviation =
 *          2.
 * https://oeis.org/A238660
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) counts partitions of n with population standard deviation =
 *            2. The algorithm iterates over all k | n^2, then counts
 *            partitions into k parts with required sum of squares using a
 *            recursive DFS with strong pruning
 * Range:     u8 n <= 15, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Validated: partial, pass, 71 terms through n = 71 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Maximum sum of squares for a given total sum, number of parts, and
   an upper bound on each part. The greedy algorithm (assign as much as
   possible to the first part, then repeat) gives the optimum because
   the square function is convex. */
static seq_t max_sq(idx_t sum, idx_t parts, idx_t maxPart) {
    seq_t total = 0;
    idx_t rem_sum = sum;
    idx_t rem_parts = parts;
    while (rem_parts) {
        /* largest part we can choose ensuring at least 1 for each remaining part */
        idx_t take = rem_sum - (rem_parts - 1);
        if (take > maxPart) take = maxPart;
        if (take < 1) take = 1;              /* safety */
        rem_sum -= take;
        rem_parts--;
        total += (seq_t)take * take;
    }
    return total;
}

/* Minimum sum of squares for a given total sum and number of parts.
   All parts are positive integers and at most <= maxPart (it is always satisfied
   in the recursion). */
static seq_t min_sq(idx_t sum, idx_t parts) {
    idx_t q = sum / parts;
    idx_t r = sum % parts;
    seq_t sq = ((seq_t)(parts - r)) * q * q + (seq_t)r * (q + 1) * (q + 1);
    return sq;
}

/* forward declarations of helper functions defined in utils (if needed) */

/* Recursive DFS counting partitions of `sum` into `parts` parts, each <= `maxPart`,
   whose sum of squares equals `sq`. */
static seq_t dfs(idx_t sum, idx_t parts, seq_t sq, idx_t maxPart) {
    if (parts == 0) {
        return (sum == 0 && sq == 0) ? (seq_t)1 : (seq_t)0;
    }
    if (parts == 1) {
        /* only one part left */
        if (sum <= maxPart && sq == (seq_t)sum * sum)
            return (seq_t)1;
        else
            return (seq_t)0;
    }

    seq_t minSq = min_sq(sum, parts);
    seq_t maxSq = max_sq(sum, parts, maxPart);
    if (sq < minSq || sq > maxSq) return (seq_t)0;

    /* bounds for the next part value (nonincreasing sequence) */
    idx_t low  = (sum + parts - 1) / parts;            /* ceil(avg)      */
    idx_t high = sum - (parts - 1);                    /* max possible  */
    if (high > maxPart) high = maxPart;

    seq_t count = 0;
    for (idx_t p = high; p >= low; --p) {
        seq_t p_sq = (seq_t)p * p;
        if (p_sq > sq) continue;                     /* too large */
        idx_t rem_sum  = sum - p;
        idx_t rem_parts = parts - 1;
        seq_t rem_sq = sq - p_sq;

        seq_t minSqRem = min_sq(rem_sum, rem_parts);
        seq_t maxSqRem = max_sq(rem_sum, rem_parts, p);
        if (rem_sq < minSqRem || rem_sq > maxSqRem) continue;

        count += dfs(rem_sum, rem_parts, rem_sq, p);
    }
    return count;
}

seq_t a(idx_t n) {
    seq_t total = 0;
    if (n == 0) return total;

    seq_t n_sq = (seq_t)n * (seq_t)n;

    for (idx_t k = 1; k <= n; ++k) {
        if ((uint64_t)n_sq % k != 0) continue;          /* n^2 must be divisible by k */

        seq_t target_sq = n_sq / k + 4 * (seq_t)k;

        seq_t minSqAll = min_sq((idx_t)n, k);
        seq_t maxSqAll = max_sq((idx_t)n, k, (idx_t)n);
        if (target_sq < minSqAll || target_sq > maxSqAll) continue;

        total += dfs((idx_t)n, k, target_sq, (idx_t)n);
    }
    return total;
}
