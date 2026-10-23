/*
 * A179002  Primes p such that 2^(2p-1) mod (2p+1) is prime.
 * https://oeis.org/A179002
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   is_prime_big, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the nth prime p such that 2^(2p-1) mod (2p+1) is prime.
 * The compute is done by incremental generation of primes and testing the
 * property; results are memoised, so any call order and n up to about 10 000
 * works within the time limits. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/is_prime_big.h"   /* bool is_prime_big(seq_t n); */
#include "../utils/mod_pow.h"        /* seq_t mod_pow(seq_t base, uint64_t exp, seq_t mod); */

/* ---- cache of qualifying primes (answers) ---- */
static seq_t *answers     = NULL;   /* array of qualifying primes */
static size_t answers_cnt = 0;      /* number of stored answers */
/* helper to append to the answers cache */
static void append_answer(seq_t val)
{
    if (answers_cnt == 0) {
        answers = (seq_t*)malloc(8 * sizeof(seq_t));
        if (!answers) exit(1);
        answers_cnt   = 0;
    } else if (answers_cnt >= 8) {
        /* grow on the fly; the initial size 8 is arbitrary and can be
         * expanded whenever the count is a multiple of the capacity */
        size_t new_cap = (answers_cnt < 16 ? 16 : answers_cnt << 1);
        seq_t *tmp = (seq_t*)realloc(answers, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        answers = tmp;
    }
    answers[answers_cnt++] = val;
}

/* ---- state for incremental prime generation ---- */
static seq_t next_candidate = 2;   /* next odd number to test for primality */
/* The following loop generates only odd numbers after 2, because all
   even numbers >2 are composite. */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                  /* offset 1: undefined for n==0 */
    while (answers_cnt < (size_t)n) {       /* generate until we have n answers */

        /* Test whether the current candidate is a prime. */
        if (is_prime_big(next_candidate)) {

            /* Compute m = 2p + 1 in seq_t and the exponent e = 2p - 1 in uint64_t.
               For the expected range of n (up to 10 000) p stays well below
               64 bit limits, so converting to uint64_t is safe. */
            seq_t m = (seq_t)( (next_candidate << 1u) + 1 );
            uint64_t e = (uint64_t)((next_candidate << 1u) - 1);

            /* Compute r = 2^e mod m. The helper mod_pow works for arbitrary
               precision seq_t types and returns a seq_t. */
            seq_t r = mod_pow((seq_t)2, e, m);

            /* If r is prime, the candidate satisfies the definition of the
               sequence, so store it. */
            if (is_prime_big(r))
                append_answer(next_candidate);
        }

        /* Advance to the next candidate; after 2 we skip even numbers. */
        if (next_candidate == 2)
            next_candidate = 3;
        else
            next_candidate += 2;
    }

    return answers[(size_t)n - 1];
}
