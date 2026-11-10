/*
 * A066905  Squares in A006577.
 * https://oeis.org/A066905
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th term of A006577 (in index order) that is a perfect
 *            square; computed by search
 * Range:     u8 n <= 9, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Validated: partial, pass, 68 terms through n = 68 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static seq_t A006577_a(uint64_t m)
{
    seq_t x = (seq_t)m;
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

static unsigned long long *A066905_found = 0;
static size_t A066905_cnt = 0;
static size_t A066905_cap = 0;
static uint64_t A066905_next = 1;

static int A066905_is_sq(unsigned long long v)
{
    unsigned long long r = 0;
    while (r * r < v) r++;
    return r * r == v;
}

seq_t a(idx_t n)
{
    while (A066905_cnt < (size_t)n) {
        seq_t s = A006577_a(A066905_next);
        unsigned long long sv = (unsigned long long)s;
        A066905_next++;
        if (A066905_is_sq(sv)) {
            if (A066905_cnt == A066905_cap) {
                size_t nc = A066905_cap ? A066905_cap * 2 : 64;
                unsigned long long *p = (unsigned long long *)realloc(A066905_found, nc * sizeof(unsigned long long));
                if (!p) return (seq_t)0;
                A066905_found = p;
                A066905_cap = nc;
            }
            A066905_found[A066905_cnt++] = sv;
        }
    }
    return (seq_t)A066905_found[(size_t)n - 1];
}
