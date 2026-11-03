/*
 * A217656  Primes p such that p = 361 + 420*k for some k.
 * https://oeis.org/A217656
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   this sequence enumerates primes of the form 361 + 420·k. a(n) is
 *            the n‑th prime in that arithmetic progression
 * Range:     u8 none, u16 n <= 37, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Validated: partial, pass, 37 terms through n = 37 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
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

/* Append a value to the cached array, expanding it if necessary.  `p` is a
   pointer to the array pointer. */
static void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *p, newcap);
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        *p = tmp;
        *cap = newcap;
    }
    (*p)[*len] = val;
    (*len)++;
}

seq_t a(idx_t n)
{
    /* Static cache of found primes in the progression 361 + 420·k */
    static seq_t *primes   = NULL;   /* array of primes           */
    static size_t count    = 0;      /* number of primes stored   */
    static size_t capacity = 0;      /* current capacity of arr   */
    static idx_t next_k    = 0;      /* next k to test            */

    if (n < 1) return 0;            /* undefined, return 0 */

    /* Generate primes until we have at least n entries */
    while (count < n) {
        /* p = 361 + 420·k  */
        seq_t p = 361 + ((seq_t)420 * (seq_t)next_k);
        if (isPrimeGeneric(p))
            append_candidate(&primes, &count, &capacity, p);
        ++next_k;
    }

    return primes[(size_t)(n - 1)];   /* 0‑based indexing */
}
