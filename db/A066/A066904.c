/*
 * A066904  Triangular numbers in A006577.
 * https://oeis.org/A066904
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th (from n=0) term of A006577(m), m>=1, that is a
 *            positive triangular number; found by scanning m upward and
 *            caching
 * Range:     u8 n <= 9, u16 n <= 627, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Validated: standard, pass, 82 terms through n = 1939 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static seq_t A006577_a(idx_t n)
{
    seq_t x = (seq_t)n;
    seq_t steps = 0;
    while (x != 1) {
        if (x % 2 == 0) {
            x = x / 2;
        } else {
            x = 3 * x + 1;
        }
        steps++;
    }
    return steps;
}

static int A066904_is_tri(unsigned long long v)
{
    unsigned long long t, r;
    if (v == 0) return 0;
    t = 8ULL * v + 1ULL;
    r = 0;
    {
        unsigned long long lo = 0, hi = 1ULL << 32;
        while (lo < hi) {
            unsigned long long mid = lo + (hi - lo + 1) / 2;
            if (mid * mid <= t) lo = mid; else hi = mid - 1;
        }
        r = lo;
    }
    return r * r == t;
}

static unsigned long long *A066904_res = 0;
static size_t A066904_cnt = 0, A066904_cap = 0;
static idx_t A066904_next = 1;

seq_t a(idx_t n)
{
    while (A066904_cnt <= (size_t)n) {
        seq_t s = A006577_a(A066904_next);
        unsigned long long v = (unsigned long long)s;
        A066904_next++;
        if (A066904_is_tri(v)) {
            if (A066904_cnt == A066904_cap) {
                A066904_cap = A066904_cap ? A066904_cap * 2 : 1024;
                A066904_res = (unsigned long long *)realloc(A066904_res, A066904_cap * sizeof(unsigned long long));
            }
            A066904_res[A066904_cnt++] = v;
        }
    }
    return (seq_t)A066904_res[(size_t)n];
}
