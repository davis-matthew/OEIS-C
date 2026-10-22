/*
 * A005379  The male of a pair of recurrences.
 * https://oeis.org/A005379
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   F(0) = 1, M(0) = 0; M(n) = n - F(M(n-1)), F(n) = n - M(F(n-1));
 *            a(n) = M(n)
 * Range:     u8 n <= 413, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint64_t *F_memo = NULL;
static uint64_t *M_memo = NULL;
static idx_t memo_size = 0;
static idx_t memo_cap = 0;

static void compute_up_to(idx_t n) {
    if (memo_size > n) return;

    if (memo_cap < n + 1) {
        idx_t new_cap = memo_cap ? memo_cap : 64;
        while (new_cap < n + 1) new_cap *= 2;
        F_memo = (uint64_t *)realloc(F_memo, new_cap * sizeof(uint64_t));
        M_memo = (uint64_t *)realloc(M_memo, new_cap * sizeof(uint64_t));
        memo_cap = new_cap;
    }

    if (memo_size == 0) {
        F_memo[0] = 1;
        M_memo[0] = 0;
        memo_size = 1;
    }

    for (idx_t i = memo_size; i <= n; i++) {
        M_memo[i] = (uint64_t)i - F_memo[M_memo[i - 1]];
        F_memo[i] = (uint64_t)i - M_memo[F_memo[i - 1]];
    }

    memo_size = n + 1;
}

seq_t a(idx_t n) {
    compute_up_to(n);
    return (seq_t)M_memo[n];
}
