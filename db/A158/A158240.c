/*
 * A158240  Smallest number made up of n consecutive digits such that every
 *          k-digit substring (k <= n) taken from the left is divisible by k
 *          (k=1..n).
 * https://oeis.org/A158240
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   dfs_backtrack
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by backtracking search over permutations of consecutive distinct digits
   with the prefix‑divisibility property (each leftmost k‑digit substring is
   divisible by k). */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/dfs_backtrack.h"

seq_t a(idx_t n)
{
    /* trivial cases */
    if (n == 0)
        return (seq_t)0;
    if (n == 1)
        return (seq_t)1;

    /* with distinct decimal digits we cannot have n > 10 */
    if (n > 10)
        return (seq_t)0;

    /* try all possible starting digits of the consecutive set */
    for (uint8_t start = 0; start <= 10 - (uint8_t)n; ++start) {
        uint8_t digits[10];
        bool used[10];
        for (idx_t i = 0; i < n; ++i) {
            digits[i] = start + (uint8_t)i;
            used[i] = false;
        }
        seq_t result;
        if (dfs_backtrack(digits, used, n, 0, (seq_t)0, &result))
            return result;
    }
    return (seq_t)0;   /* no solution for this n */
}
