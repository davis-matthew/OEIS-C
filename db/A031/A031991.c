/*
 * A031991  Smallest n-digit norep emirp.
 * https://oeis.org/A031991
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Validated: full, pass, 9 terms through n = 9 (2026-11-03)
 * Helpers:   is_prime_u64, reverse_digits
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
#include <stdint.h>
#include <stdlib.h>
#include "../utils/reverse_digits.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* Recursive helper that searches for the first valid candidate.
   Position `pos` is the current digit index (0‑based). */
static bool dfs(idx_t pos, idx_t n, bool used[10], int digits[10], seq_t *found) {
    if (pos == n) {
        /* Construct the integer from the collected digits. */
        seq_t val = 0;
        for (idx_t i = 0; i < n; ++i)
            val = val * 10 + (seq_t)digits[i];

        seq_t rev = reverse_digits(val);
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
