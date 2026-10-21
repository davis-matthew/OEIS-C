/*
 * A096458  If n is prime, the next prime after the next prime after n.
 *          otherwise n.
 * https://oeis.org/A096458
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n, if n is not prime; otherwise the next prime after the
 *            next prime after n
 * Range:     u8 n <= 240, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  n is 1-indexed as per OEIS offset 1. */
#include <stdbool.h>
#include "../utils/is_prime.h"

static uint64_t next_prime_uint64(uint64_t x)
{
    /* Return the smallest prime strictly greater than x. */
    uint64_t cand = x + 1;
    if (cand <= 2) return 2;
    if ((cand & 1ULL) == 0) cand++;                  /* make odd */
    for (;; cand += 2) {
        if (is_prime((idx_t)cand)) return cand;
    }
}

seq_t a(idx_t n)
{
    if (is_prime((idx_t)n)) {
        uint64_t p1 = next_prime_uint64((uint64_t)n);
        uint64_t p2 = next_prime_uint64(p1);
        return (seq_t)p2;
    }
    return (seq_t)n;
}
