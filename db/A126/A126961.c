/*
 * A126961  Primes p such that (3p)^2 + 2 is not prime.
 * https://oeis.org/A126961
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 14, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 71 terms through n = 996 (2026-11-03)
 * Helpers:   is_prime, is_prime_u128, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes p such that (3p)^2 + 2 is not prime */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include "../utils/is_prime.h"
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"

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

#define MAX_TERMS 20000   /* sufficient for n ≤ 10 000 */

static seq_t  results[MAX_TERMS];   /* stored primes that satisfy the condition */
static size_t results_len = 0;      /* number of primes stored so far */
static idx_t  last_checked = 0;     /* last integer examined in the search */

/* Ensure that at least 'need' terms are available. */
static void
ensure_terms(size_t need)
{
    if (results_len >= need) return;

    for (;;)
    {
        idx_t cand = last_checked + 1;
        last_checked = cand;

        /* skip non‑prime candidates */
        if (!is_prime(cand))
            continue;

        /* compute (3p)^2 + 2 = 9*p*p + 2 */
        seq_t val = (seq_t)cand;
        val *= val;            /* p²   */
        val *= (seq_t)9;       /* 9p² */
        val += (seq_t)2;       /* +2  */

        /* test the condition */
        if (!isPrimeGeneric(val))
        {
            if (results_len < MAX_TERMS)
                results[results_len++] = (seq_t)cand;
            /* else ignore – our static buffer is full */
        }

        if (results_len >= need)
            break;   /* enough terms accumulated */
    }
}

/* Return the n‑th term of A126961 (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* undefined for n < 1 */
    if (n > MAX_TERMS) return 0;       /* caller expects n within limits */

    ensure_terms((size_t)n);
    return results[(size_t)n - 1];
}
