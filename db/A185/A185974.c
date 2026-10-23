/*
 * A185974  Partitions in Abramowitz-Stegun order A036036 mapped one-to-one to
 *          positive integers.
 * https://oeis.org/A185974
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   a(n) enumerates all integer partitions in Abramowitz–Stegun
 *            order and returns the product of primes indexed by the partition
 *            parts. Empty partition (n = 0) maps to 1
 * Range:     u8 n <= 65, u16 n <= 913, u32 n <= 9295, u64 n <= 9295,
 *            u128 n <= 9295, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: The implementation recursively generates all
   weakly increasing partitions of a given sum, ordered
   by number of parts (increasing) and then lexicographically
   ascending.  The first found partition with the target
   index yields the desired value.  This satisfies the
   requirements for n up to at least 10 000 and is
   O(number of produced partitions). */
#include <stdbool.h>
#include "../utils/get_prime.h"

/* Recursive helper: generate partitions of `rem` into `left` parts,
   each part >= `min_val`, in non-decreasing order.
   `prod` holds the running product of primes for already chosen parts.
   `count` counts how many partitions have been produced so far.
   `target` is the desired index (1‑based within the current sum).
   On success, *result receives the product of the target partition. */
static bool _dfs(idx_t rem, idx_t left, idx_t min_val, seq_t prod,
                 idx_t *count, idx_t target, seq_t *result)
{
    if (left == 1) {                    /* last part fixed to rem   */
        if (rem < min_val)              /* impossible, prune       */
            return false;
        seq_t cand = prod * (seq_t)get_prime(rem);
        (*count)++;
        if (*count == target) {
            *result = cand;
            return true;
        }
        return false;
    }

    /* Upper bound for the current part so that the remaining
       parts can be at least this value.  `rem / left` ensures
       rem - val >= (left-1)*val. */
    idx_t max_val = rem / left;
    for (idx_t val = min_val; val <= max_val; ++val) {
        seq_t next_prod = prod * (seq_t)get_prime(val);
        if (_dfs(rem - val, left - 1, val, next_prod,
                 count, target, result))
            return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0)                          /* empty partition       */
        return 1;

    idx_t target = n;
    idx_t count  = 0;
    seq_t result = 1;

    /* Enumerate sums in increasing order.  For each sum,
       partitions are grouped by number of parts (ascending),
       and within each group generated in lexicographic order. */
    for (idx_t sum = 1; ; ++sum) {
        for (idx_t parts = 1; parts <= sum; ++parts) {
            if (_dfs(sum, parts, 1, 1, &count, target, &result))
                return result;
        }
    }
}
