/*
 * A229019  Minimal position at which the sequence defined in the same way as
 *          A159559 but with initial term prime(n) merges with A159559; a(n)=0
 *          if there is no such position.
 * https://oeis.org/A229019
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k >= 2 with B_n(k) = A159559(k), where B_n is built
 *            like A159559 but with B_n(2) = prime(n); found by direct
 *            simulation of both sequences (0 if none is found)
 * Range:     u8 n <= 6, u16 n <= 37, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Validated: partial, pass, 44 terms through n = 45 (2026-11-07)
 * Helpers:   is_prime, is_prime_u64, nth_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/is_prime_u64.h"
#include "../utils/nth_prime.h"

static bool A229019_isp(seq_t v)
{
    if (sizeof(seq_t) <= 8) return is_prime_u64((uint64_t)v);
    if (v <= (seq_t)UINT64_MAX) return is_prime_u64((uint64_t)v);
    /* values this large are not expected here */
    return false;
}

static seq_t A229019_next(seq_t prev, bool need_prime)
{
    seq_t cur = prev + 1;
    while (A229019_isp(cur) != need_prime) ++cur;
    return cur;
}

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;
    seq_t p = (seq_t)3;
    seq_t q = (seq_t)nth_prime(n);
    if (p == q) return (seq_t)2;
    for (idx_t k = 3; k < 20000000ULL; ++k) {
        bool np = is_prime(k);
        p = A229019_next(p, np);
        q = A229019_next(q, np);
        if (p == q) return (seq_t)k;
    }
    return (seq_t)0;
}
