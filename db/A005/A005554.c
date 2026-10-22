/*
 * A005554  Sums of successive Motzkin numbers.
 * https://oeis.org/A005554
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   (n+1)*a(n) = 2*n*a(n-1) + (3*n-9)*a(n-2), a(1) = 1, a(2) = 2.
 *            Rewritten as a(n) = 2*a(n-1) + 3*a(n-2) - (2*a(n-1) +
 *            12*a(n-2))/(n+1) so that the division never overflows and the
 *            rest is exact mod 2^w
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 24, u64 n <= 46, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *memo = NULL;
static idx_t memo_len = 0;   /* memo[1..memo_len-1] are valid */
static idx_t memo_cap = 0;

seq_t a(idx_t n) {
    if (n == 1) return 1;
    if (n == 2) return 2;

    if (n >= memo_cap) {
        idx_t new_cap = (n < 1000) ? 1000 : n + 1;
        seq_t *nm = (seq_t *)malloc((size_t)(new_cap + 1) * sizeof(seq_t));
        idx_t i;
        for (i = 0; i <= new_cap; i++) nm[i] = 0;
        for (i = 0; i < memo_len; i++) nm[i] = memo[i];
        if (memo) free(memo);
        memo = nm;
        memo_cap = new_cap;
    }

    if (memo_len < 3) {
        memo[0] = 0;
        memo[1] = 1;
        memo[2] = 2;
        memo_len = 3;
    }

    while (memo_len <= n) {
        idx_t i = memo_len;
        seq_t d = (seq_t)i + 1;
        seq_t prev = memo[i - 1];
        seq_t pp = memo[i - 2];
        seq_t p1 = prev / d, s1 = prev % d;
        seq_t p2 = pp / d, s2 = pp % d;
        seq_t q = (seq_t)2 * p1 + (seq_t)12 * p2 + ((seq_t)2 * s1 + (seq_t)12 * s2) / d;
        memo[i] = (seq_t)2 * prev + (seq_t)3 * pp - q;
        memo_len++;
    }

    return memo[n];
}
