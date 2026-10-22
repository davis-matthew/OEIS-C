/*
 * A003160  a(1) = a(2) = 1, a(n) = n - a(a(n-1)) - a(a(n-2)).
 * https://oeis.org/A003160
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - a(a(n-1)) - a(a(n-2)), a(1) = a(2) = 1
 * Range:     u8 n <= 509, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint64_t *memo = 0;
static idx_t memo_cnt = 0; /* memo[1..memo_cnt-1] are computed */
static idx_t memo_cap = 0;

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;
    if (n < 3) return (seq_t)1;
    if (n >= memo_cap) {
        idx_t newcap = memo_cap ? memo_cap * 2 : 1024;
        if (newcap <= n) newcap = n + 1;
        uint64_t *p = (uint64_t *)realloc(memo, (size_t)newcap * sizeof(uint64_t));
        if (!p) return (seq_t)0;
        memo = p;
        memo_cap = newcap;
    }
    if (memo_cnt < 3) {
        memo[0] = 0;
        memo[1] = 1;
        memo[2] = 1;
        memo_cnt = 3;
    }
    while (memo_cnt <= n) {
        idx_t i = memo_cnt;
        memo[i] = (uint64_t)i - memo[memo[i - 1]] - memo[memo[i - 2]];
        memo_cnt++;
    }
    return (seq_t)memo[n];
}
