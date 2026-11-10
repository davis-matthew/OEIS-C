/*
 * A214784  Numbers n such that n-4=A214783(n).
 * https://oeis.org/A214784
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th number m with A214783(m) = m-4, where A214783(m) =
 *            smallest k such that m divides Fibonacci(k-1)+3; found by
 *            incremental search with memoized terms
 * Range:     u8 n <= 9, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Validated: partial, pass, 41 terms through n = 41 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t *A214784_terms = 0;
static uint64_t A214784_cnt = 0, A214784_cap = 0, A214784_next = 5;

static int A214784_ok(uint64_t m)
{
    uint64_t target = m - 4;
    uint64_t fp = 0, fc = 1 % m, k;
    for (k = 1; k <= target; ++k) {
        if ((fp + 3) % m == 0)
            return k == target;
        uint64_t nx = (fp + fc) % m;
        fp = fc;
        fc = nx;
    }
    return 0;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    while (A214784_cnt < (uint64_t)n) {
        uint64_t m = A214784_next++;
        if (A214784_ok(m)) {
            if (A214784_cnt == A214784_cap) {
                A214784_cap = A214784_cap ? A214784_cap * 2 : 64;
                A214784_terms = (uint64_t *)realloc(A214784_terms, A214784_cap * sizeof(uint64_t));
            }
            A214784_terms[A214784_cnt++] = m;
        }
    }
    return (seq_t)A214784_terms[n - 1];
}
