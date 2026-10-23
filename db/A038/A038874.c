/*
 * A038874  Primes p such that 3 is a square mod p.
 * https://oeis.org/A038874
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A038874: primes p such that 3 is a square residue modulo p.
   Generates the sequence by testing primes in order.  No hard‑coded table
   is used; the sequence is computed on demand and cached for repeated
   calls. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* static storage for the list of all primes found so far  */
static uint64_t *primes      = NULL;
static size_t   primes_len   = 0;
static size_t   primes_cap   = 0;

/* static storage for the list of primes satisfying the condition   */
static seq_t *special        = NULL;
static size_t   special_len  = 0;
static size_t   special_cap  = 0;

/* next integer to test for primality                                 */
static uint64_t next_candidate = 2;

/* ------------------------------------------------------------------ */
/* utility to grow the primes array                                  */
static void grow_primes(void)
{
    if (primes_cap == 0) {
        primes_cap = 16;
        primes = (uint64_t *)malloc(primes_cap * sizeof(uint64_t));
    } else {
        primes_cap *= 2;
        primes = (uint64_t *)realloc(primes, primes_cap * sizeof(uint64_t));
    }
}

/* utility to grow the special array                                  */
static void grow_special(void)
{
    if (special_cap == 0) {
        special_cap = 16;
        special = (seq_t *)malloc(special_cap * sizeof(seq_t));
    } else {
        special_cap *= 2;
        special = (seq_t *)realloc(special, special_cap * sizeof(seq_t));
    }
}

/* ------------------------------------------------------------------ */
/* ensure that the list `special` contains at least `n` elements      */
static void ensure_special(idx_t n)
{
    /* Grow storage if necessary */
    if (primes_cap == 0) grow_primes();
    if (special_cap == 0) grow_special();

    while (special_len < n) {
        uint64_t cand = next_candidate;

        /* Test cand for primality using previously found primes */
        bool is_prime = true;
        if (cand < 2)
            is_prime = false;
        else {
            for (size_t i = 0; i < primes_len; ++i) {
                uint64_t p = primes[i];
                if (p > cand / p)          /* p*p > cand */
                    break;
                if (cand % p == 0) {
                    is_prime = false;
                    break;
                }
            }
        }

        if (is_prime) {
            /* store new prime */
            if (primes_len == primes_cap) grow_primes();
            primes[primes_len++] = cand;

            /* check the condition: p == 2 or 3 or p ≡ 1 or 11 (mod 12) */
            if (cand == 2 || cand == 3 ||
                cand % 12 == 1 || cand % 12 == 11) {
                if (special_len == special_cap) grow_special();
                special[special_len++] = (seq_t)cand;
            }
        }

        ++next_candidate;
    }
}

/* ------------------------------------------------------------------ */
/* public function                                                     */
seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* offset is 1 – behaviour below is irrelevant */
    ensure_special(n);
    return special[n - 1];
}
