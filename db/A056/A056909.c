/*
 * A056909  Primes of the form k^2+6.
 * https://oeis.org/A056909
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 20, u32 n <= 2341, u64 n <= 2400,
 *            u128 n <= 2400, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A056909: a(n) is the n-th prime of the form k²+6. */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/mod_pow.h"

/* Deterministic Miller–Rabin test for 64‑bit integers.  The candidate
   numbers k²+6 are always < 2⁶⁴ for k up to about 2³², so this test is
   sufficient.  It uses the same arithmetics as the general seq_t type
   (integer multiply, divide, modulo). */
static bool isPrimeSeq(seq_t n)
{
    if (n < 2)
        return false;

    /* Trial division by small primes */
    static const uint32_t small_primes[] = {
        2u, 3u, 5u, 7u, 11u, 13u, 17u, 19u,
        23u, 29u, 31u, 37u, 41u, 43u, 47u, 53u,
        59u, 61u, 67u, 71u, 73u, 79u, 83u, 89u,
        97u
    };
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        seq_t p = (seq_t)small_primes[i];
        if (n == p)
            return true;
        if (n % p == 0)
            return false;
    }

    /* Write n‑1 as d·2^s with d odd */
    seq_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }

    /* Deterministic bases for 64‑bit integers */
    static const uint32_t bases[] = {2u, 3u, 5u, 7u, 11u, 13u, 17u};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        seq_t a = (seq_t)bases[i];
        if (a % n == 0)
            continue;
        seq_t x = mod_pow(a, (uint64_t)d, n);
        if (x == 1 || x == n - 1)
            continue;
        bool composite = true;
        for (int r = 1; r < s; ++r) {
            x = (x * x) % n;
            if (x == n - 1) {
                composite = false;
                break;
            }
        }
        if (composite)
            return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* Static state to allow arbitrary call order */
    static seq_t *primes = NULL;
    static size_t primes_cap = 0;
    static idx_t  primes_sz  = 0;
    static idx_t  next_k     = 1;        /* next integer k to test */

    while (primes_sz < n) {
        seq_t k = (seq_t)next_k;
        seq_t candidate = k * k + (seq_t)6;

        if (isPrimeSeq(candidate)) {
            /* Grow the array if necessary */
            if (primes_sz == primes_cap) {
                size_t new_cap = primes_cap ? primes_cap << 1 : 8;
                seq_t *tmp = (seq_t *)realloc(primes, new_cap * sizeof(seq_t));
                if (!tmp) {
                    /* Allocation failure – abort so the caller sees a crash */
                    exit(1);
                }
                primes     = tmp;
                primes_cap = new_cap;
            }
            primes[primes_sz++] = candidate;
        }
        ++next_k;
    }

    return primes[n - 1];
}
