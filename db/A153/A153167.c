/*
 * A153167  Numbers n such that n+2 is not a Chen prime.
 * https://oeis.org/A153167
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th term of OEIS sequence A153167:
 * numbers n such that n+2 is *not* a Chen prime. */

#include <stdbool.h>
#include <stdint.h>

/* Test whether x is a prime number.  Works for all x that fit in a 64‑bit
 * unsigned integer. */
static bool is_prime(uint64_t x) {
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    for (uint64_t d = 3; d * d <= x; d += 2)
        if (x % d == 0)
            return false;
    return true;
}

/* Test whether x is a semiprime (product of exactly two primes,
 * possibly equal).  Works for x that fit in a 64‑bit unsigned integer. */
static bool is_semiprime(uint64_t x) {
    if (x < 4) return false;          /* 4 = 2*2 is the smallest semiprime */
    unsigned int cnt = 0;
    uint64_t tmp = x;
    for (uint64_t d = 2; d * d <= tmp; ++d) {
        while (tmp % d == 0) {
            ++cnt;
            tmp /= d;
            if (cnt > 2) return false;
        }
    }
    if (tmp > 1) ++cnt;                /* remaining prime factor */
    return cnt == 2;
}

/* a(idx_t n) returns the nth member of A153167 (offset 1). */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;      /* undefined for n<1, return 0 */
    idx_t found = 0;
    seq_t candidate = 1;              /* start before first positive integer */
    while (1) {
        ++candidate;
        idx_t cand64 = (idx_t)candidate;
        bool ok = false;

        if ((cand64 & 1U) == 0) {     /* even numbers always qualify */
            ok = true;
        } else {
            uint64_t p = (uint64_t)cand64 + 2ULL;   /* n+2 */
            if (!is_prime(p)) {
                ok = true;
            } else {
                uint64_t q = p + 2ULL;                /* p+2 = n+4 */
                if (is_prime(q) || is_semiprime(q))
                    ok = false;
                else
                    ok = true;
            }
        }

        if (ok) {
            ++found;
            if (found == n)
                return candidate;
        }
    }
}
