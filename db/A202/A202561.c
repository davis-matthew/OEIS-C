/*
 * A202561  Length of non-periodic part of the sequence defined by the
 *          recursion in A202211 and starting with a(1)=prime(n),
 *          a(2)=prime(n+1).
 * https://oeis.org/A202561
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   with a(1)=prime(n), a(2)=prime(n+1), t(k) = 2*(gpf(t(k-1)) +
 *            gpf(t(k-2))) + 1 (A202211 recursion); a(n) = number of terms
 *            before the periodic part begins (cycle detection on consecutive
 *            pairs, Floyd's algorithm)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 9801 (2026-11-07)
 * Helpers:   largest_prime_factor, next_prime, nth_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/largest_prime_factor.h"
#include "../utils/nth_prime.h"
#include "../utils/next_prime.h"

typedef struct { uint64_t x, y; } A202561_st;

static A202561_st A202561_step(A202561_st s)
{
    A202561_st r;
    r.x = s.y;
    r.y = 2 * (largest_prime_factor(s.y) + largest_prime_factor(s.x)) + 1;
    return r;
}

static bool A202561_eq(A202561_st a, A202561_st b)
{
    return a.x == b.x && a.y == b.y;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    A202561_st s0;
    s0.x = nth_prime(n);
    s0.y = next_prime(s0.x);

    A202561_st t = A202561_step(s0);
    A202561_st h = A202561_step(t);
    while (!A202561_eq(t, h)) {
        t = A202561_step(t);
        h = A202561_step(A202561_step(h));
    }
    t = s0;
    uint64_t mu = 0;
    while (!A202561_eq(t, h)) {
        t = A202561_step(t);
        h = A202561_step(h);
        mu++;
    }
    return (seq_t)mu;
}
