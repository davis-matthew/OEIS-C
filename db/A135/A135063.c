/*
 * A135063  Define the sequence {b_n(m)} by b_n(0)=0; b_n(m) =
 *          A000005(b_n(m-1)+n), for all m >= 1. Then a(n) is the smallest
 *          positive integer such that b_n(m) = b_n(m + a(n)) for all m > some
 *          positive integer.
 * https://oeis.org/A135063
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the eventual cycle length of the sequence b_n(0)=0,
 *            b_n(m)=d(b_n(m-1)+n) (d=number of divisors). Computed via
 *            Floyd's cycle‑finding algorithm
 * Range:     u8 n <= 51, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

static seq_t divisor_count(seq_t m)
{
    if (m <= 1) return 1;
    seq_t result = 1;
    seq_t i = 2;
    seq_t limit = floor_sqrt(m);
    while (i <= limit) {
        if (m % i == 0) {
            seq_t e = 0;
            while (m % i == 0) {
                m /= i;
                ++e;
            }
            result *= (e + 1);
            limit = floor_sqrt(m);
        }
        ++i;
    }
    if (m > 1) result *= 2;
    return result;
}

static seq_t next_value(seq_t x, seq_t n)
{
    /* f(x) = d(x + n) */
    return divisor_count(x + n);
}

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;           /* promote n for arithmetic */
    /* tortoise–hare cycle detection */
    seq_t tortoise = next_value(0, sn);
    seq_t hare = next_value(tortoise, sn);
    while (tortoise != hare) {
        tortoise = next_value(tortoise, sn);
        hare = next_value(next_value(hare, sn), sn);
    }
    /* found meeting point: compute cycle length λ */
    seq_t lambda = 1;
    hare = next_value(tortoise, sn);
    while (hare != tortoise) {
        hare = next_value(hare, sn);
        ++lambda;
    }
    return lambda;
}
