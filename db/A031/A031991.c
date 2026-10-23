/*
 * A031991  Smallest n-digit norep emirp.
 * https://oeis.org/A031991
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 4, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   is_prime_big, reverse_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: search for the smallest n‑digit norep emirp
   (OEIS A031991).  The search explores all permutations of distinct
   digits in ascending numeric order, prunes trailing zeros, and tests
   primality of the number and its decimal reversal.  For n>10 no such
   number exists, so the function returns 0. */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"
#include "../utils/reverse_decimal.h"

/* Recursive helper that searches for the first valid candidate.
   Position `pos` is the current digit index (0‑based). */
static bool dfs(idx_t pos, idx_t n, bool used[10], int digits[10], seq_t *found) {
    if (pos == n) {
        /* Construct the integer from the collected digits. */
        seq_t val = 0;
        for (idx_t i = 0; i < n; ++i)
            val = val * 10 + (seq_t)digits[i];

        seq_t rev = reverse_decimal(val);
        if (is_prime_big(val) && is_prime_big(rev)) {
            *found = val;
            return true;
        }
        return false;
    }

    /* First position: digits 1..9. */
    if (pos == 0) {
        for (int d = 1; d <= 9; ++d) {
            if (!used[d]) {
                used[d] = true;
                digits[pos] = d;
                if (dfs(pos + 1, n, used, digits, found))
                    return true;
                used[d] = false;
            }
        }
        return false;
    }

    /* Last position (for n > 1): digits 1,3,7,9. */
    if (pos == n - 1) {
        /* Only consider odd, non‑zero digits suitable for a prime ending. */
        const int last_digits[4] = {1, 3, 7, 9};
        for (int i = 0; i < 4; ++i) {
            int d = last_digits[i];
            if (!used[d]) {
                used[d] = true;
                digits[pos] = d;
                if (dfs(pos + 1, n, used, digits, found))
                    return true;
                used[d] = false;
            }
        }
        return false;
    }

    /* Middle positions: digits 0..9 distinct. */
    for (int d = 0; d <= 9; ++d) {
        if (!used[d]) {
            used[d] = true;
            digits[pos] = d;
            if (dfs(pos + 1, n, used, digits, found))
                return true;
            used[d] = false;
        }
    }
    return false;
}

/* Wrapper that performs the search for a specific n. */
static seq_t find_smallest_emirp(idx_t n) {
    bool used[10] = {false};
    int digits[10] = {0};
    seq_t answer = 0;
    dfs(0, n, used, digits, &answer);
    return answer;
}

/* Public function required by the framework. */
seq_t a(idx_t n) {
    if (n == 0)
        return 0;                 /* offset is 1 */
    if (n > 10)
        return 0;                 /* no distinct‑digit number with >10 digits */

    /* Memoise results for repeated calls. */
    static seq_t memo[11] = {0};
    static bool computed[11] = {false};

    if (!computed[n]) {
        memo[n] = find_smallest_emirp(n);
        computed[n] = true;
    }
    return memo[n];
}
