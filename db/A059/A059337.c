/*
 * A059337  Primes p such that x^35 = 2 has no solution mod p.
 * https://oeis.org/A059337
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime p such that x^35 = 2 has no solution modulo p
 *            (A059337)
 * Range:     u8 n <= 16, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   append_candidate, gcd, is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime.h"
#include "../utils/gcd.h"
#include "../utils/mod_pow.h"
#include "../utils/append_candidate.h"

static seq_t *found = NULL;       /* array of collected primes */
static size_t found_len = 0;      /* number of primes stored */
static size_t found_cap = 0;      /* capacity of the array */
static uint64_t search_start = 3; /* next candidate to test (odd numbers only) */

/* Extend the list of primes until we have at least n entries. */
static void expand_to(idx_t n) {
    while (found_len < (size_t)n) {
        for (uint64_t cand = search_start; ; cand += 2) {
            if (!is_prime((idx_t)cand)) continue;
            if ((cand % 5 != 1) && (cand % 7 != 1)) continue;

            seq_t g   = gcd((seq_t)35, (seq_t)(cand - 1));
            seq_t exp = ((seq_t)(cand - 1)) / g;
            seq_t res = mod_pow((seq_t)2, (uint64_t)exp, (seq_t)cand);

            if (res != 1) {                       /* no 35th‑root of 2 */
                append_candidate(&found, &found_len, &found_cap, (seq_t)cand);
                search_start = cand + 2;           /* next search starts after this prime */
                break;                            /* move on to find the next prime */
            }
        }
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined for n < 1, but harmless */
    expand_to(n);
    return found[(size_t)n - 1];
}
