/*
 * A086678  Numbers n such that 1 + Sum_{k=1..n} 2k^5 is a prime.
 * https://oeis.org/A086678
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the nth integer m such that (2*m^6+6*m^5+5*m^4-m^2+6)/6
 *            is prime
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 10, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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

static seq_t *terms = NULL;   /* cached terms (the m’s) */
static size_t terms_len = 0;  /* how many terms are stored */
static size_t terms_cap = 0;  /* capacity of the cache array */
static idx_t checked_n = 0;   /* largest m that has been examined */

/* Append a new term to the cache, expanding the array if necessary. */
static inline void add_term(seq_t val)
{
    if (terms_len + 1 > terms_cap) {
        terms_cap = terms_cap ? terms_cap * 2 : 16;
        terms = mem_resize(seq_t, terms, terms_cap);
    }
    terms[terms_len++] = val;
}

/* Compute the nth term of A086678. */
seq_t a(idx_t n)
{
    /* Generate terms until we have at least n of them. */
    while (terms_len < n) {
        ++checked_n;                      /* next candidate integer m */

        /* Use 128‑bit arithmetic for the intermediate calculation
           to avoid overflow on 64‑bit builds. */
        unsigned __int128 N  = (unsigned __int128)checked_n;
        unsigned __int128 N2 = N * N;
        unsigned __int128 N4 = N2 * N2;
        unsigned __int128 N5 = N4 * N;
        unsigned __int128 N6 = N5 * N;

        unsigned __int128 numer = 2 * N6 + 6 * N5 + 5 * N4 + 6;
        numer -= N2;                     /* subtract n^2 */

        /* Divisibility by 6 is guaranteed by the formula. */
        unsigned __int128 val128 = numer / 6;
        seq_t val = (seq_t)val128;       /* cast to seq_t (safe for arbitrary‑precision) */

        if (isPrimeGeneric(val)) {
            add_term((seq_t)checked_n);  /* store the candidate m itself */
        }
    }

    return terms[n - 1];                 /* return the nth term (1‑based). */
}
