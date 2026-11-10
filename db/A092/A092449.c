/*
 * A092449  Composite numbers in A092447.
 * https://oeis.org/A092449
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   a(k) = k-th composite term of A092447 (A092447(n) = primes
 *            p(n+3), ..., 5, 3 concatenated); found by scanning n and testing
 *            primality
 * Range:     u8 none, u16 n <= 2, u32 n <= 3, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Validated: partial, pass, 11 terms through n = 11 (2026-11-07)
 * Helpers:   is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_seq.h"

static bool A092449_small_prime(uint64_t x)
{
    if (x < 2) return false;
    for (uint64_t d = 2; d * d <= x; d++)
        if (x % d == 0) return false;
    return true;
}

seq_t a(idx_t n)
{
    seq_t cur = 753;
    seq_t pw = 1000;
    uint64_t cand = 11;
    idx_t count = 0;

    for (;;) {
        if (!is_prime_seq(cur)) {
            count++;
            if (count == n) return cur;
        }
        while (!A092449_small_prime(cand)) cand++;
        uint64_t p = cand;
        cand++;
        cur = (seq_t)p * pw + cur;
        uint64_t t = p;
        while (t) { pw = pw * 10; t /= 10; }
    }
}
