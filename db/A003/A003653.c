/*
 * A003653  Discriminants of quadratic fields whose fundamental unit has norm
 *          -1.
 * https://oeis.org/A003653
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search over fundamental discriminants D > 1 (D = 1 mod 4
 *            squarefree, or D = 4d with d = 2,3 mod 4 squarefree); keep D
 *            when the continued fraction of (1+sqrt(D))/2 (resp. sqrt(d)) has
 *            odd period length, i.e. the fundamental unit has norm -1
 * Range:     u8 n <= 32, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   a000037_isqrt, sqfree_u
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/a000037_isqrt.h"
#include "../utils/sqfree_u.h"

static uint64_t *memo = NULL;
static idx_t memo_len = 0, memo_cap = 0;
static uint64_t last_D = 4;

/* Parity of the period of the continued fraction of (P0 + sqrt(N)) / Q0 */
static bool odd_period(uint64_t N, int64_t P0, int64_t Q0) {
    int64_t s = (int64_t)a000037_isqrt(N);
    int64_t P = P0, Q = Q0;
    /* one step to reach a reduced (purely periodic) state */
    int64_t q = (P + s) / Q;
    P = q * Q - P;
    Q = ((int64_t)N - P * P) / Q;
    int64_t P1 = P, Q1 = Q;
    uint64_t len = 0;
    do {
        q = (P + s) / Q;
        P = q * Q - P;
        Q = ((int64_t)N - P * P) / Q;
        len++;
    } while (!(P == P1 && Q == Q1) && len < 100000000ULL);
    return (len & 1) != 0;
}

static bool qualifies(uint64_t D) {
    if (D % 4 == 1) {
        if (!sqfree_u(D)) return false;
        return odd_period(D, 1, 2);
    }
    if (D % 4 == 0) {
        uint64_t d = D / 4;
        if (d % 4 != 2 && d % 4 != 3) return false;
        if (!sqfree_u(d)) return false;
        return odd_period(d, 0, 1);
    }
    return false;
}

seq_t a(idx_t n) {
    while (memo_len < n) {
        uint64_t D = last_D + 1;
        while (!qualifies(D)) D++;
        last_D = D;
        if (memo_len == memo_cap) {
            memo_cap = memo_cap ? memo_cap * 2 : 1024;
            memo = (uint64_t *)realloc(memo, memo_cap * sizeof(uint64_t));
        }
        memo[memo_len++] = D;
    }
    return (seq_t)memo[n - 1];
}
