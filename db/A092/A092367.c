/*
 * A092367  Numbers n with property that the binary cyclic right-rotating
 *          progressive sum of digits (see A091821) is exactly n/2.
 * https://oeis.org/A092367
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = n-th even m such that A091821(m) = m/2, where A091821(m)
 *            = floor(m/L)*popcount(m) + popcount(m >> (L - m mod L)), L =
 *            bitlength(m); found by a cached linear search
 * Range:     u8 n <= 21, u16 n <= 37, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Validated: partial, pass, 37 terms through n = 37 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t *A092367_terms = 0;
static uint64_t A092367_cnt = 0;
static uint64_t A092367_cap = 0;
static uint64_t A092367_next = 2;

static int A092367_ok(uint64_t m)
{
    unsigned ones = (unsigned)__builtin_popcountll(m);
    unsigned len = 64 - (unsigned)__builtin_clzll(m);
    uint64_t full = m / len;
    uint64_t rem = m % len;
    uint64_t res = full * ones;
    if (rem) res += (unsigned)__builtin_popcountll(m >> (len - rem));
    return 2 * res == m;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    while (A092367_cnt < n) {
        uint64_t m = A092367_next;
        A092367_next += 2;
        if (A092367_ok(m)) {
            if (A092367_cnt == A092367_cap) {
                A092367_cap = A092367_cap ? A092367_cap * 2 : 1024;
                A092367_terms = (uint64_t *)realloc(A092367_terms, A092367_cap * sizeof(uint64_t));
            }
            A092367_terms[A092367_cnt++] = m;
        }
    }
    return (seq_t)A092367_terms[n - 1];
}
