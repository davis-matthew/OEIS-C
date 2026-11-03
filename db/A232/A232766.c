/*
 * A232766  Numbers n such that the concatenation n987654321 is prime.
 * https://oeis.org/A232766
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) consists of all integers n such that the concatenation
 *            n987654321 is prime. a(1) = 28, a(2) = 32, ... This function
 *            computes the n‑th such integer
 * Range:     u8 none, u16 none, u32 none, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Validated: partial, pass, 53 terms through n = 53 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return is_prime_u64((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return is_prime_u128((__uint128_t)n);
#else
    /* fallback: if seq_t > 64‑bit but no 128‑bit support,
       order the test as simple trial division up to sqrt */
    seq_t limit = 0;
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
}

seq_t a(idx_t n)
{
    /* static cache of already discovered terms */
    static seq_t *cache = NULL;
    static size_t capacity = 0;   /* current allocated array size   */
    static size_t count   = 0;    /* number of terms stored         */
    static idx_t next_candidate = 1; /* smallest n not yet examined */

    /* generate terms until we have at least n of them */
    while (count < n) {
        /* concatenate candidate with 987654321 : candidate*10^9 + 987654321 */
        seq_t concat = (seq_t)next_candidate * 1000000000ULL + 987654321ULL;

        /* test primality of the concatenated number */
        if (isPrimeGeneric(concat)) {
            /* grow cache if necessary */
            if (count == capacity) {
                size_t newcap = capacity ? capacity * 2 : 8;
                seq_t *tmp = mem_resize(seq_t, cache, newcap);
                if (!tmp) {
                    /* out of memory – terminate the program */
                    abort();
                }
                cache = tmp;
                capacity = newcap;
            }
            cache[count++] = next_candidate;
        }
        ++next_candidate;
    }

    return cache[n - 1];   /* cache[0] is a(1) */
}
