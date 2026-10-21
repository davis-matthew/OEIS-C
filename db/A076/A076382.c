/*
 * A076382  Numbers k whose sum of digits in base 4 is a divisor of the sum of
 *          its prime divisors (A008472).
 * https://oeis.org/A076382
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 38, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   sum_base4_digits, sum_distinct_prime_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A076382: the n‑th integer k such that (sum of base‑4 digits of k) divides
   (sum of distinct prime divisors of k). */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/sum_base4_digits.h"
#include "../utils/sum_distinct_prime_divisors.h"

/* integer square‑root for 64‑bit values */
static uint64_t floor_isqrt_u64(uint64_t n)
{
    uint64_t res = 0;
    uint64_t bit = 1ULL << 62;
    while (bit != 0) {
        if (n >= res + bit) {
            n -= res + bit;
            res = (res >> 1) + bit;
        } else {
            res >>= 1;
        }
        bit >>= 2;
    }
    return res;
}

/* expand the global prime list up to (and including) limit.
   The list grows only as needed. */
static void generate_primes_up_to(uint64_t limit,
                                  uint64_t **pr, uint64_t *cnt, uint64_t *cap)
{
    static uint64_t last_prime = 0;
    static bool init = false;

    /* initialise with the first prime */
    if (!init) {
        *cap = 4;
        *pr = (uint64_t *)realloc(*pr, *cap * sizeof(uint64_t));
        (*pr)[0] = 2;
        *cnt = 1;
        last_prime = 2;
        init = true;
    }
    if (*cnt > 0 && last_prime >= limit)
        return;

    uint64_t cand = last_prime + 1;
    if ((cand & 1U) == 0) cand++;          /* start with an odd number */

    for (; cand <= limit; cand += 2) {
        bool prime = true;
        uint64_t sqrt_c = floor_isqrt_u64(cand);
        for (uint64_t i = 0; i < *cnt; ++i) {
            uint64_t p = (*pr)[i];
            if (p > sqrt_c) break;
            if (cand % p == 0) { prime = false; break; }
        }
        if (prime) {
            if (*cnt == *cap) {
                *cap = (*cap == 0) ? 8 : (*cap << 1);
                *pr = (uint64_t *)realloc(*pr, *cap * sizeof(uint64_t));
            }
            (*pr)[(*cnt)++] = cand;
            last_prime = cand;
        }
    }
}

/* the required sequence function */
seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *terms = NULL;
    static uint64_t terms_len = 0, terms_cap = 0;

    static uint64_t *prime_list = NULL;
    static uint64_t prime_cnt = 0, prime_cap = 0;

    static uint64_t candidate = 2;        /* next integer to test */

    /* collect terms until we have at least n terms */
    while (terms_len < n) {
        /* ensure primes up to sqrt(candidate) */
        uint64_t sqrt_cand = floor_isqrt_u64(candidate);
        generate_primes_up_to(sqrt_cand, &prime_list, &prime_cnt, &prime_cap);

        uint64_t s4 = sum_base4_digits(candidate);
        if (s4 != 0) {
            uint64_t sdp = sum_distinct_prime_divisors(candidate,
                                                       prime_list, prime_cnt);
            if (sdp % s4 == 0) {
                /* add to terms array */
                if (terms_len == terms_cap) {
                    terms_cap = (terms_cap == 0) ? 16 : (terms_cap << 1);
                    terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
                }
                terms[terms_len++] = (seq_t)candidate;
            }
        }
        ++candidate;
    }
    return terms[n - 1];
}
