/*
 * A005376  a(n) = n - a(a(a(a(a(n-1))))).
 * https://oeis.org/A005376
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - a(a(a(a(a(n-1))))), a(0) = 0
 * Range:     u8 n <= 339, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *memo = NULL;
    static idx_t memo_len = 0;
    static idx_t computed_len = 0;

    if (n < computed_len) {
        return memo[n];
    }

    if (memo == NULL) {
        memo_len = (n < 1000) ? 1000 : (n + 100);
        memo = (seq_t *)malloc(sizeof(seq_t) * memo_len);
        memo[0] = 0;
        computed_len = 1;
    }

    if (n >= memo_len) {
        idx_t newlen = memo_len;
        while (n >= newlen) {
            newlen *= 2;
        }
        memo = (seq_t *)realloc(memo, sizeof(seq_t) * newlen);
        memo_len = newlen;
    }

    for (idx_t i = computed_len; i <= n; i++) {
        idx_t v = i - 1;
        for (int j = 0; j < 5; j++) {
            v = (idx_t)(long long)memo[v];
        }
        memo[i] = (seq_t)i - memo[0] - (seq_t)v;
    }

    computed_len = n + 1;

    return memo[n];
}
