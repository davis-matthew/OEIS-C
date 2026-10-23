/*
 * A028416  Primes p such that the decimal expansion of 1/p has a periodic
 *          part of even length.
 * https://oeis.org/A028416
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mod_pow, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A028416: primes p such that 1/p has an even period
 * length (i.e. the multiplicative order of 10 modulo p is even).  The
 * function searches for successive primes and stores those satisfying the
 * condition in a dynamic array.  No table of known values is hard‑coded. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "../utils/mod_pow.h"    /* seq_t mod_pow(seq_t base, uint64_t exp, seq_t mod); */
#include "../utils/next_prime.h" /* uint64_t next_prime(uint64_t start); */

static seq_t *good = NULL;          /* list of primes with even order */
static size_t good_len = 0, good_cap = 0;
static uint64_t last_checked = 1;   /* last prime examined */

/* Grow a dynamic array of seq_t */
static void grow_array(seq_t **arr, size_t *len, size_t *cap)
{
    if (*len == *cap) {
        *cap = (*cap == 0) ? 16 : (*cap << 1);
        *arr = (seq_t *)realloc(*arr, *cap * sizeof(seq_t));
    }
}

/* Return true if 10 has even multiplicative order modulo p (p is prime). */
static bool has_even_order(seq_t p)
{
    if (p == 2 || p == 5)
        return false;

    uint64_t order = (uint64_t)p - 1;     /* p fits in seq_t; order fits in uint64_t */
    uint64_t n     = order;
    uint64_t q;

    /* trial division of n (p-1) */
    for (q = 2; q * q <= n; ++q) {
        if (n % q == 0) {
            while (n % q == 0) n /= q;   /* remove the factor from n */
            /* try to shorten order by this prime factor */
            while (order % q == 0) {
                uint64_t cand = order / q;
                seq_t r = mod_pow((seq_t)10, cand, p);
                if (r == 1)
                    order = cand;
                else
                    break;
            }
        }
    }
    /* remaining prime factor > sqrt(n) */
    if (n > 1) {
        q = n;
        while (order % q == 0) {
            uint64_t cand = order / q;
            seq_t r = mod_pow((seq_t)10, cand, p);
            if (r == 1)
                order = cand;
            else
                break;
        }
    }
    return (order & 1ULL) == 0;  /* true if order even */
}

/* Initialise storage on first use. */
static void ensure_tables(void)
{
    if (good != NULL)
        return;
    good_cap = 16;
    good = (seq_t *)malloc(good_cap * sizeof(seq_t));
    good_len = 0;
    last_checked = 1;
}

/* Find the next prime after last_checked that satisfies the condition
 * and append it to the good array.  Update last_checked each time. */
static void find_next(void)
{
    uint64_t cand = next_prime(last_checked + 1);  /* next prime > last_checked */

    /* test and store the new prime */
    if (cand != 2 && cand != 5 && has_even_order((seq_t)cand)) {
        grow_array(&good, &good_len, &good_cap);
        good[good_len++] = (seq_t)cand;
    }

    last_checked = cand;   /* advance anchor for next search */
}

/* Public entry point required by the test harness. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                    /* undefined at index 0, safe default */

    ensure_tables();

    while (good_len < (size_t)n)
        find_next();                 /* keep generating until we have n good terms */

    return good[(size_t)n - 1];
}
