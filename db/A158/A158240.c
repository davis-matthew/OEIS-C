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
 * Validated: full, pass, 10 terms through n = 10 (2026-11-02)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by backtracking search over permutations of consecutive distinct digits
   with the prefix‑divisibility property (each leftmost k‑digit substring is
   divisible by k). */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

/* Recursive helper: tries to build the number in lexicographic order.
   digits   – sorted list of the n consecutive digits to use
   used[i]  – whether digit i has been placed already
   pos      – current length of the prefix built so far
   prefix   – numeric value of the current prefix
   result   – pointer to store the final number when a full solution is found
*/
static bool dfs_backtrack(const uint8_t *digits, bool *used,
                          idx_t n, idx_t pos, seq_t prefix, seq_t *result)
{
    if (pos == n) {
        *result = prefix;
        return true;
    }
    idx_t len = pos + 1;
    seq_t mod_len = (seq_t)len;
    for (idx_t i = 0; i < n; ++i) {
        if (!used[i]) {
            uint8_t d = digits[i];
            if (pos == 0 && n > 1 && d == 0)
                continue;          /* leading zero is forbidden for n>1 */
            seq_t newp = prefix * (seq_t)10 + (seq_t)d;
            if (newp % mod_len != 0)
                continue;
            used[i] = true;
            if (dfs_backtrack(digits, used, n, pos + 1, newp, result))
                return true;
            used[i] = false;
        }
    }
    return false;
}

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
