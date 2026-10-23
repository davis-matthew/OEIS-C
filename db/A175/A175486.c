/*
 * A175486  Composite numbers of form 8n+5 with all prime factors of form
 *          8m+5.
 * https://oeis.org/A175486
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A175486: Composite numbers of the form 8k+5 whose all prime
    factors are ≡5 (mod 8).  
    The algorithm tests successive numbers 5,13,21,29,… and
    keeps those that are composite and whose every prime factor
    satisfies p % 8 == 5.  It stores the found terms in a static
    array so that calls for arbitrary indices are handled
    efficiently. */
    
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/isPrimeGeneric.h"

static bool initialized = false;
static seq_t *seq = NULL;          /* dynamic array of terms   */
static size_t seq_len = 0;         /* number of terms stored   */
static size_t seq_cap = 0;         /* capacity of seq array    */
static seq_t next_candidate = 5;   /* first 8k+5 number to test */

static uint64_t *primes = NULL;    /* list of all primes up to the sieve limit */
static size_t primes_cnt = 0;

/* ------------------------------------------------------------------ */
/*  Simple sieve: generates all primes up to `limit` (inclusive).     */
static void ensure_primes(uint64_t limit)
{
    if (limit <= 1)
        return;

    bool *is_composite = (bool *)calloc(limit + 1, sizeof(bool));
    if (!is_composite)
        exit(EXIT_FAILURE);

    for (uint64_t i = 2; i * i <= limit; ++i) {
        if (!is_composite[i]) {
            for (uint64_t j = i * i; j <= limit; j += i)
                is_composite[j] = true;
        }
    }
    for (uint64_t i = 2; i <= limit; ++i) {
        if (!is_composite[i]) {
            uint64_t *tmp = (uint64_t *)realloc(primes,
                                                 (primes_cnt + 1) * sizeof(uint64_t));
            if (!tmp)
                exit(EXIT_FAILURE);
            primes = tmp;
            primes[primes_cnt++] = i;
        }
    }
    free(is_composite);
}

/* ------------------------------------------------------------------ */
/*  Initialise static data (called on first invocation).             */
static void ensure_initialized(void)
{
    if (initialized)
        return;

    /* A sieve limit that covers the prime factors needed for the
       first few thousand terms.  200 000 is ample for n up to 10 000. */
    ensure_primes(200000);

    seq = NULL;
    seq_len = 0;
    seq_cap = 0;
    next_candidate = 5;          /* first 8k+5 number */

    initialized = true;
}

/* ------------------------------------------------------------------ */
/*  Append a new term to the cached sequence.                         */
static void append_term(seq_t val)
{
    if (seq_len == seq_cap) {
        size_t new_cap = (seq_cap == 0) ? 16 : seq_cap << 1;
        seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        if (!tmp)
            exit(EXIT_FAILURE);
        seq = tmp;
        seq_cap = new_cap;
    }
    seq[seq_len++] = val;
}

/* ------------------------------------------------------------------ */
/*  Test whether a number `m` satisfies the definition.               */
static bool good_candidate(seq_t m)
{
    /* Reject primes */
    if (isPrimeGeneric(m))
        return false;

    seq_t temp = m;
    bool composite = false;

    /* Trial division by all primes found by the sieve.            */
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p64 = primes[i];
        seq_t p = (seq_t)p64;

        /* stop once p*p > temp */
        if (p * p > temp)
            break;

        if (temp % p == 0) {
            composite = true;
            /* Remove all copies of p */
            while (temp % p == 0)
                temp /= p;

            /* Factor must be ≡5 (mod 8) */
            if ((p64 & 7ULL) != 5ULL)
                return false;
        }
    }

    /* Remaining factor, if any, is prime and larger than the sieve limit. */
    if (temp > (seq_t)1) {
        composite = true;
        seq_t rem_mod = temp % (seq_t)8;
        if ((uint64_t)rem_mod != 5ULL)
            return false;
    }

    return composite;
}

/* ------------------------------------------------------------------ */
/*  Main sequence function.                                           */
seq_t a(idx_t n)
{
    ensure_initialized();

    while (seq_len < (size_t)n) {
        if (good_candidate(next_candidate))
            append_term(next_candidate);
        next_candidate += 8;
    }
    return seq[(size_t)n - 1];
}
