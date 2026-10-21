/*
 * A211686  Prime numbers > 10000 such that all the substrings of length >= 4
 *          are primes (substrings with leading '0' are considered to be
 *          nonprime).
 * https://oeis.org/A211686
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 164, u32 n <= 263, u64 n <= 263, u128 n <= 263,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   A211686: Prime numbers > 10000 such that every decimal substring
   of length ≥ 4 is prime (substrings that start with 0 are not prime).
   The sequence is finite; this implementation builds the list once
   and returns the n‑th term (1‑based).  If n is beyond the last term,
   0 is returned.
*/

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* see the build system for the definitions of seq_t and idx_t */
extern idx_t _prime_limit_for_idx(idx_t n); /* unused but present in utils */

/*                 ---  static state  ------------------------------------- */
static seq_t *term_cache   = NULL;   /* dynamically allocated array of terms */
static size_t term_count   = 0;      /* number of stored terms               */
static size_t term_cap     = 0;      /* current capacity of the array        */
static uint8_t *prime_sieve = NULL;  /* 1 = prime, 0 = composite            */
static const size_t SIEVE_MAX = 1000000;   /* sieve up to 999 999 (enough)     */
static bool init_done = false;

/*                 ---  helper  ------------------------------------------- */
static void init_sequence(void)
{
    if (init_done) return;
    init_done = true;

    /*---  Build the sieve for primality tests --------------------------------*/
    prime_sieve = (uint8_t*)calloc(SIEVE_MAX, sizeof(uint8_t));
    if (!prime_sieve) return;          /* allocation failure – initialization aborts */

    memset(prime_sieve, 1, SIEVE_MAX);   /* assume all are prime */
    prime_sieve[0] = prime_sieve[1] = 0;  /* 0 and 1 are not prime */

    for (size_t i = 2; i * i < SIEVE_MAX; ++i) {
        if (prime_sieve[i]) {
            for (size_t j = i * i; j < SIEVE_MAX; j += i)
                prime_sieve[j] = 0;
        }
    }

    /*---  Reserve space for the terms ------------------------------------*/
    term_cap = 512;  /* start with a modest capacity */
    term_cache = (seq_t*)malloc(term_cap * sizeof(seq_t));
    if (!term_cache) return;   /* allocation failure – abort */

    /*---  Scan primes greater than 10 000 and keep only the good ones ------*/
    for (size_t p = 10001; p < SIEVE_MAX; ++p) {
        if (!prime_sieve[p]) continue;   /* not prime, skip */

        /* Extract decimal digits of p into digits[0 .. len-1] (most significant first) */
        uint8_t digits[7];    /* p < 1 000 000 → at most 6 digits, spare one for safety */
        size_t len = 0;
        uint32_t tmp = (uint32_t)p;

        /* Build digits in reverse order */
        uint8_t rev[7];
        size_t rev_len = 0;
        while (tmp > 0) {
            rev[rev_len++] = tmp % 10;
            tmp /= 10;
        }
        /* Reverse to obtain normal order */
        for (size_t i = 0; i < rev_len; ++i)
            digits[i] = rev[rev_len - 1 - i];
        len = rev_len;

        /* Verify all contiguous substrings of length >= 4 are prime */
        bool ok = true;
        for (size_t i = 0; i + 3 < len && ok; ++i) {
            uint32_t val = 0;
            for (size_t j = i; j < len; ++j) {
                val = val * 10 + digits[j];

                size_t sub_len = j - i + 1;
                if (sub_len >= 4) {
                    if (digits[i] == 0) {        /* leading zero → non‑prime */
                        ok = false;
                        break;
                    }
                    /* val ≤ 999 999 because p < 1 000 000 */
                    if (prime_sieve[val] == 0) {
                        ok = false;
                        break;
                    }
                }
            }
        }

        if (ok) {
            /* Store the valid prime in the cache */
            if (term_count == term_cap) {
                term_cap *= 2;
                seq_t *tmp = (seq_t*)realloc(term_cache, term_cap * sizeof(seq_t));
                if (!tmp) return; /* allocation failure – abort */
                term_cache = tmp;
            }
            term_cache[term_count++] = (seq_t)p;   /* implicit conversion allowed */
        }
    }
}

/*             ---  The requested function --------------------------------- */
seq_t a(idx_t n)
{
    init_sequence();          /* lazy initialization */

    if (n == 0) return 0;     /* sequence is 1‑based */

    if ((idx_t)term_count >= n)
        return term_cache[n - 1];

    /* Sequence is finite; values beyond the end are undefined.
       Returning 0 is safest for callers that do not check the index. */
    return 0;
}
