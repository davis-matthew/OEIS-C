/*
 * A084445  Primes that cannot be written as 1+p+p^k, p prime and k > 0.
 * https://oeis.org/A084445
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 38, u16 n <= 6045, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_power_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = the n‑th prime that cannot be written as
 * 1 + p + p^k with p prime and k > 0.
 * The function generates primes by trial division and tests each
 * candidate for such a representation.  It works for any build
 * where seq_t and idx_t are defined. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_power_of.h"

/* ---------- Helper: trial division primality test ---------- */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2)          return false;
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0)   return false;
    for (uint64_t d = 3; d * d <= n; d += 2)
        if (n % d == 0) return false;
    return true;
}

/* ---------- Check whether r = 1 + p + p^k has a solution ---------- */
static bool has_representation(uint64_t r)
{
    /* 2 and 3 cannot be written as 1+p+p^k with k>0 */
    if (r <= 3) return false;

    uint64_t v   = r - 1;      /* r - 1 = p + p^k = p(1 + p^(k-1))          */
    uint64_t rem = v;          /* rem will be factorised into primes        */

    for (uint64_t p = 2; p * p <= rem; ++p) {
        if (rem % p == 0) {
            uint64_t m = v / p - 1;            /* candidate value for p^(k-1) */
            if (m > 0 && is_power_of(m, p)) return true;
            while (rem % p == 0) rem /= p;     /* remove this prime factor   */
        }
    }
    if (rem > 1) {                            /* remaining prime factor      */
        uint64_t p = rem;
        uint64_t m = v / p - 1;
        if (m > 0 && is_power_of(m, p)) return true;
    }
    return false;
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    seq_t target = 0;       /* will hold the n‑th term */
    idx_t  found  = 0;
    uint64_t candidate = 2; /* start from the first prime */

    while (found < n) {
        if (is_prime_uint64(candidate) && !has_representation(candidate)) {
            ++found;
            if (found == n)
                target = (seq_t)candidate;
        }
        ++candidate;
    }
    return target;
}
