/*
 * A237989  Numbers m such that the numbers of primes, even positive integers
 *          and odd positive integers less than or equal to m are all odd.
 * https://oeis.org/A237989
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 8060, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Condition: m must be ≡ 2 (mod 4) and the count of primes ≤ m is odd. */
/* The first few terms are: 2, 6, 18, 26, 34, 42, … */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static seq_t *terms = NULL;          /* values of the sequence          */
static size_t terms_cap = 0;         /* capacity of terms array         */
static size_t terms_size = 0;        /* number of terms stored          */

static uint8_t *prime_flags = NULL;  /* 1 = prime, 0 = composite        */
static uint64_t sieve_limit = 0;     /* maximum m for which primes are stored */
static uint64_t last_checked_m = 0;  /* largest m already examined        */
static uint8_t  parity_pi = 0;       /* parity of π(last_checked_m)      */

/* ------------------------------------------------------------------ */
/* Simple sieve that recomputes all primes up to 'limit'.              */
/* ------------------------------------------------------------------ */
static void sieve_up_to(uint64_t limit)
{
    free(prime_flags);
    prime_flags = (uint8_t*)malloc((limit + 1) * sizeof(uint8_t));
    if (!prime_flags) abort();

    for (uint64_t i = 0; i <= limit; ++i) prime_flags[i] = 1;
    if (limit >= 0) prime_flags[0] = 0;
    if (limit >= 1) prime_flags[1] = 0;

    for (uint64_t i = 2; i * i <= limit; ++i)
    {
        if (prime_flags[i])
        {
            for (uint64_t j = i * i; j <= limit; j += i)
                prime_flags[j] = 0;
        }
    }
    sieve_limit = limit;
}

/* ------------------------------------------------------------------ */
/* Extend the cached terms until at least 'n' terms are available.    */
/* ------------------------------------------------------------------ */
static void extend_terms(idx_t n)
{
    /* Ensure we have enough range to search for more terms. */
    while (terms_size < (size_t)n)
    {
        /* If the sieve is too small, grow it (doubling strategy). */
        if (sieve_limit < last_checked_m + 1 || sieve_limit == 0)
        {
            uint64_t new_limit = (sieve_limit == 0) ? 1000 : sieve_limit * 2;
            if (new_limit < last_checked_m + 1) new_limit = last_checked_m + 1;
            sieve_up_to(new_limit);
        }

        /* Recompute the parity of π(last_checked_m).  Since
           sieve_up_to may have rebuilt the whole table, we recompute this
           count from scratch; it is small (≤ limit). */
        parity_pi = 0;
        for (uint64_t i = 1; i <= last_checked_m; ++i)
            if (prime_flags[i]) parity_pi ^= 1;

        /* Scan new m's up to the current sieve limit. */
        uint64_t m;
        for (m = last_checked_m + 1; m <= sieve_limit; ++m)
        {
            if (prime_flags[m]) parity_pi ^= 1;

            if ((m & 3) == 2 && parity_pi == 1)   /* m ≡ 2 (mod 4) and π(m) odd */
            {
                if (terms_size == terms_cap)
                {
                    size_t new_cap = terms_cap ? terms_cap * 2 : 4;
                    seq_t *tmp = (seq_t*)realloc(terms, new_cap * sizeof(seq_t));
                    if (!tmp) abort();
                    terms = tmp;
                    terms_cap = new_cap;
                }
                terms[terms_size++] = (seq_t)m;
                if (terms_size >= (size_t)n) break;
            }
        }
        last_checked_m = m;
    }
}

/* ------------------------------------------------------------------ */
/* Return the nth term of A237989 (1‑based indexing).                  */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* out of bounds, defensive */
    extend_terms(n);
    return terms[n - 1];
}
