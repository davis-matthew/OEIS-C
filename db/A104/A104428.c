/*
 * A104428  Numbers n such that n, p[n], p[n]+n, p[n]-n and p[n]*n all are
 *          8-less.
 * https://oeis.org/A104428
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 37, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   has_digit8
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A104428
 * The nth term is the nth integer n such that
 * n, p[n], p[n] + n, p[n] – n and p[n] * n contain no decimal digit 8,
 * where p[n] is the nth prime.
 * The terms are found by a simple incremental search using
 * trial‑division to generate primes on demand. */

#include <stddef.h>   /* size_t, NULL */
#include <stdlib.h>   /* malloc, realloc, free */
#include <stdbool.h>  /* bool, true, false */
#include <stdint.h>   /* uint64_t */
#include "../utils/has_digit8.h"

 

/***************** Prime generator (trial division) ******************/

static uint64_t *prime_arr  = NULL;   /* array of primes */
static size_t    prime_cap  = 0;
static size_t    prime_len  = 0;
static uint64_t  next_prime = 2;       /* next integer to test */

static void grow_prime_array(void)
{
    prime_cap = prime_cap ? prime_cap << 1 : 16;
    uint64_t *tmp = (uint64_t *)realloc(prime_arr, prime_cap * sizeof(uint64_t));
    if (!tmp) abort(); /* allocation failure */
    prime_arr = tmp;
}

static void generate_next_prime(void)
{
    for (;; ++next_prime)
    {
        bool ok = true;
        for (size_t i = 0; i < prime_len &&
             prime_arr[i] * prime_arr[i] <= next_prime; ++i)
        {
            if (next_prime % prime_arr[i] == 0)
            {
                ok = false;
                break;
            }
        }
        if (ok)
        {
            if (prime_len == prime_cap)
                grow_prime_array();
            prime_arr[prime_len++] = next_prime;
            ++next_prime;
            return;
        }
    }
}

static void ensure_primes_up_to(idx_t idx)
{
    while (prime_len < idx)
        generate_next_prime();
}

/****************  Sequence storage  *******************************/

static uint64_t *term_arr  = NULL; /* collected n that satisfy the property */
static size_t    term_cap  = 0;
static size_t    term_len  = 0;
static uint64_t  next_candidate = 1; /* next integer to test */

static void grow_term_array(void)
{
    term_cap = term_cap ? term_cap << 1 : 16;
    uint64_t *tmp = (uint64_t *)realloc(term_arr, term_cap * sizeof(uint64_t));
    if (!tmp) abort();
    term_arr = tmp;
}

static void push_term(uint64_t v)
{
    if (term_len == term_cap)
        grow_term_array();
    term_arr[term_len++] = v;
}

/****************** public function a(idx_t n) ************************/

seq_t a(idx_t n)
{
    /* Initialise static data on first call. */
    if (!prime_arr)
    {
        prime_cap = 16;
        prime_arr = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
        prime_len = 0;
        next_prime = 2;
    }
    if (!term_arr)
    {
        term_cap = 16;
        term_arr = (uint64_t *)malloc(term_cap * sizeof(uint64_t));
        term_len = 0;
        next_candidate = 1;
    }

    /* Generate terms until we have at least n of them. */
    while (term_len < (size_t)n)
    {
        uint64_t cand = next_candidate;

        /* Get the cand‑th prime (1‑based). */
        ensure_primes_up_to(cand);
        uint64_t p = prime_arr[cand - 1];

        /* Check 8‑less conditions. */
        if (!has_digit8((seq_t)cand) &&
            !has_digit8((seq_t)p) &&
            !has_digit8((seq_t)(p + cand)) &&
            !has_digit8((seq_t)(p - cand)))
        {
            seq_t prod = (seq_t)p * (seq_t)cand;
            if (!has_digit8(prod))
            {
                push_term(cand);
            }
        }

        ++next_candidate;
    }

    /* Return the nth term, converting to seq_t. */
    return (seq_t)term_arr[n - 1];
}
