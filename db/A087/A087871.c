/*
 * A087871  Primes of the form (4*k + 1)^2 + (4*k + 2)^2 where k=0,1,2,3,...
 * https://oeis.org/A087871
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 14, u32 n <= 1565, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 50 terms through n = 4058 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A087871: primes of the form 32*k^2 + 24*k + 5.
   Computation is done by a straightforward prime test on each
   candidate, caching results so that calls with arbitrary order of
   indices remain efficient. */
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
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

seq_t a(idx_t n) {
    if (n == 0) return 0; /* invalid offset, never used in tests */

    static seq_t *primes = NULL;
    static size_t prime_cnt = 0;
    static size_t prime_cap = 0;
    static idx_t next_k = 0;

    while (prime_cnt < (size_t)n) {
        seq_t k = (seq_t)next_k;
        seq_t candidate = (seq_t)32 * k * k + (seq_t)24 * k + (seq_t)5;

        if (isPrimeGeneric(candidate)) {
            if (prime_cnt == prime_cap) {
                size_t new_cap = prime_cap ? prime_cap * 2 : 16;
                seq_t *tmp = mem_resize(seq_t, primes, new_cap);
                if (!tmp) abort();
                primes = tmp;
                prime_cap = new_cap;
            }
            primes[prime_cnt++] = candidate;
        }
        ++next_k;
    }
    return primes[n-1];
}
