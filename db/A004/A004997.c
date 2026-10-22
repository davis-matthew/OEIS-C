/*
 * A004997  a(n) = (6^n/n!) * Product_{k=0..n-1} (6*k + 7).
 * https://oeis.org/A004997
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*(6*n+1)*a(n-1)/n, a(0) = 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *memo = NULL;
static idx_t memo_len = 0; /* number of valid entries: memo[0..memo_len-1] */
static idx_t memo_cap = 0;

static uint64_t gcd_u64(uint64_t x, uint64_t y) {
    while (y != 0) {
        uint64_t t = x % y;
        x = y;
        y = t;
    }
    return x;
}

seq_t a(idx_t n) {
    if (memo_len == 0) {
        memo_cap = 16;
        memo = (seq_t *)malloc(memo_cap * sizeof(seq_t));
        memo[0] = 1;
        memo_len = 1;
    }
    if (n < memo_len) return memo[n];

    if (n >= memo_cap) {
        idx_t new_cap = memo_cap;
        while (new_cap <= n) new_cap *= 2;
        seq_t *nm = (seq_t *)malloc(new_cap * sizeof(seq_t));
        for (idx_t i = 0; i < memo_len; i++) nm[i] = memo[i];
        free(memo);
        memo = nm;
        memo_cap = new_cap;
    }

    for (idx_t k = memo_len; k <= n; k++) {
        seq_t prev = memo[k - 1];
        /* reduce prev and k by their gcd so no intermediate exceeds the result */
        uint64_t r = (uint64_t)(prev % (seq_t)k);
        uint64_t g = gcd_u64(k, r); /* gcd(prev, k) */
        uint64_t kg = k / g;
        uint64_t m = 6 * (6 * (uint64_t)k + 1) / kg; /* kg divides 6*(6k+1) */
        memo[k] = (prev / (seq_t)g) * (seq_t)m;
    }
    memo_len = n + 1;
    return memo[n];
}
