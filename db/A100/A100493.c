/*
 * A100493  a(n) = n + n-th semiprime.
 * https://oeis.org/A100493
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n + A001358(n) (nth semiprime)
 * Range:     u8 n <= 61, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 74 terms through n = 8967 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

/* true if x is the product of exactly two primes (possibly the same) */
static bool is_semiprime_uint64(uint64_t x)
{
    if (x < 4)
        return false;
    uint64_t d = 0;
    if (x % 2 == 0)
        d = 2;
    else {
        for (uint64_t dd = 3; dd <= x / dd; dd += 2) {
            if (x % dd == 0) {
                d = dd;
                break;
            }
        }
    }
    if (d == 0)
        return false;                      /* x is prime */
    uint64_t second = x / d;
    return is_prime_u64(second);
}

seq_t a(idx_t n) {
    /* static storage for the sequence of semiprimes */
    static seq_t *semiprimes = NULL;
    static size_t semiprimes_cap = 0;
    static idx_t semiprimes_len = 0;
    static idx_t next_candidate = 4; /* next integer to test for being a semiprime */

    /* Ensure we have at least n semiprimes computed */
    while (semiprimes_len < n) {
        /* Expand the storage if necessary */
        if (semiprimes_len >= semiprimes_cap) {
            size_t new_cap = semiprimes_cap ? semiprimes_cap * 2 : 16;
            seq_t *tmp = mem_resize(seq_t, semiprimes, new_cap);
            if (!tmp) return 0; /* allocation failed */
            semiprimes = tmp;
            semiprimes_cap = new_cap;
        }

        /* Test the current candidate */
        if (is_semiprime_uint64((uint64_t)next_candidate)) {
            semiprimes[semiprimes_len++] = (seq_t)next_candidate;
        }
        next_candidate++;
    }

    return ((seq_t)n) + semiprimes[n - 1];
}
