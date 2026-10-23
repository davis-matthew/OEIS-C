/*
 * A042966  Primes p such that x^7 = 2 has a solution mod p.
 * https://oeis.org/A042966
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 46, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Primes p such that 2 is a 7th power residue modulo p.
   A prime p satisfies the condition iff
     p = 2                                 – always accepted
     or p mod 7 != 1
     or (p ≡ 1 (mod 7) and 2^((p-1)/7) ≡ 1 (mod p)) */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/mod_pow.h"

/* static storage for primes that satisfy the condition */
static seq_t *accepted = NULL;
static size_t acc_len = 0, acc_cap = 0;

/* static storage for all primes generated so far */
static uint64_t *primes = NULL;
static size_t primes_len = 0, primes_cap = 0;

/* next odd number to test for primality */
static uint64_t next_candidate = 3;

/* enlarge a seq_t array if needed */
static void grow_seq_t(seq_t **arr, size_t *cap, size_t needed)
{
    if (*cap >= needed) return;
    size_t new_cap = *cap ? *cap * 2 : 128;
    while (new_cap < needed) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    *arr = tmp;
    *cap = new_cap;
}

/* enlarge a uint64_t array if needed */
static void grow_uint64(uint64_t **arr, size_t *cap, size_t needed)
{
    if (*cap >= needed) return;
    size_t new_cap = *cap ? *cap * 2 : 64;
    while (new_cap < needed) new_cap *= 2;
    uint64_t *tmp = (uint64_t *)realloc(*arr, new_cap * sizeof(uint64_t));
    if (!tmp) exit(EXIT_FAILURE);
    *arr = tmp;
    *cap = new_cap;
}

/* ensure that at least n accepted primes are known */
static void ensure_accepted(idx_t n)
{
    if ((idx_t)acc_len >= n) return;

    /* initialise with the first prime 2 */
    if (acc_len == 0) {
        grow_uint64(&primes, &primes_cap, 1);
        primes[primes_len++] = 2ULL;
        grow_seq_t(&accepted, &acc_cap, 1);
        accepted[acc_len++] = (seq_t)2ULL;
        if ((idx_t)acc_len >= n) return;
    }

    while ((idx_t)acc_len < n) {
        uint64_t cand = next_candidate;
        bool isprime = true;

        for (size_t i = 0; i < primes_len; ++i) {
            uint64_t p = primes[i];
            if (p > cand / p) break;
            if (cand % p == 0) { isprime = false; break; }
        }

        if (isprime) {
            /* store the new prime */
            grow_uint64(&primes, &primes_cap, primes_len + 1);
            primes[primes_len++] = cand;

            /* test the property */
            bool ok = false;
            if (cand % 7ULL != 1ULL) {
                ok = true;
            } else {
                uint64_t exp = (cand - 1ULL) / 7ULL;
                seq_t res = mod_pow((seq_t)2, exp, (seq_t)cand);
                if (res == (seq_t)1) ok = true;
            }
            if (ok) {
                grow_seq_t(&accepted, &acc_cap, acc_len + 1);
                accepted[acc_len++] = (seq_t)cand;
            }
        }

        next_candidate += 2ULL;   /* skip even numbers */
    }
}

/* public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset is 1; behaviour below unused */
    ensure_accepted(n);
    return accepted[n - 1];
}
