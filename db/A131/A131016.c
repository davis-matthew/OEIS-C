/*
 * A131016  Smallest semiprime == 1 (mod n).
 * https://oeis.org/A131016
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = MIN{k ∈ A001358 : n | (k-1)}
 * Range:     u8 n <= 48, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A131016 – Smallest semiprime ≡ 1 (mod n) */

#include <stdint.h>
#include <stdbool.h>

/* Test whether the 64‑bit unsigned integer x is a semiprime
   (product of exactly two primes, counting multiplicity). */
static bool is_semiprime(uint64_t x)
{
    if (x < 4)                      /* 4 = 2×2 is the smallest semiprime */
        return false;

    uint64_t p = 0;
    for (uint64_t d = 2; d * d <= x; ++d) {
        if (x % d == 0) {
            p = d;
            break;
        }
    }
    if (p == 0)                    /* x is prime */
        return false;

    uint64_t q = x / p;            /* remaining factor */
    /* test whether q is prime */
    for (uint64_t d = 2; d * d <= q; ++d) {
        if (q % d == 0)
            return false;          /* q is composite → x has >2 prime factors */
    }
    return true;                   /* p and q are prime → x is semiprime */
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)                    /* undefined for 0; return 0 as a safe sentinel */
        return 0;

    for (idx_t i = 1;; ++i) {
        /* compute k = 1 + n*i with no overflow for typical inputs */
        __int128 prod = (__int128)n * i + 1;
        uint64_t k = (uint64_t)prod;   /* k fits in 64 bits for all practical n */

        if (is_semiprime(k))
            return (seq_t)k;
    }
}
