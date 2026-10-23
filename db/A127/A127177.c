/*
 * A127177  Primes whose "Look And Say" descriptions from right to left (in
 *          the sense of method A, i.e., frequency followed by
 *          digit-indication) are also primes.
 * https://oeis.org/A127177
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth prime whose right‑to‑left Look‑And‑Say description is
 *            prime
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   digit_len, is_prime_big, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/pow10.h"
#include "../utils/digit_len.h"
#include "../utils/is_prime_big.h"

/* Returns the nth term of OEIS A127177. */
seq_t a(idx_t n)
{
    /* static caches -------------------------------------------------------- */
    static seq_t *term_cache = NULL;
    static size_t term_cap   = 0;
    static size_t term_cnt   = 0;

    static uint64_t *prime_cache = NULL;
    static size_t prime_cap     = 0;
    static size_t prime_cnt     = 0;
    static uint64_t next_candidate = 2;

    /* ensure term cache space                                  */
    if (term_cache == NULL) {
        term_cap = 64;
        term_cache = (seq_t *)malloc(term_cap * sizeof(seq_t));
        term_cnt   = 0;
    }

    /* ensure prime cache space                                 */
    if (prime_cache == NULL) {
        prime_cap = 64;
        prime_cache = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
        prime_cnt   = 0;
    }

    /* generate terms until we have at least n of them -------- */
    while (term_cnt < n) {
        /* ----------------------------------------------------------------
           Find the next prime number and store it in prime_cache.          */
        uint64_t p;
        while (1) {
            bool is_p = true;

            if (next_candidate < 2) {
                is_p = false;
            } else if (next_candidate == 2) {
                is_p = true;
            } else if (next_candidate % 2 == 0) {
                is_p = false;
            } else {
                for (size_t i = 0; i < prime_cnt; ++i) {
                    uint64_t pr = prime_cache[i];
                    if (pr * pr > next_candidate) break;
                    if (next_candidate % pr == 0) {
                        is_p = false;
                        break;
                    }
                }
            }

            if (is_p) {
                /* store prime for future divisor tests */
                if (prime_cnt == prime_cap) {
                    prime_cap *= 2;
                    prime_cache = (uint64_t *)realloc(prime_cache, prime_cap * sizeof(uint64_t));
                }
                prime_cache[prime_cnt++] = next_candidate;
                p = next_candidate;
                break;
            }

            /* advance to next candidate (odd after 2) */
            next_candidate = (next_candidate == 2) ? 3 : next_candidate + 2;
        }

        /* ----------------------------------------------------------------
           Compute the right‑to‑left Look‑And‑Say description of p.           */
        uint64_t tmp = p;
        seq_t cur = 0;
        int prev_digit = -1;
        int run_len = 0;

        while (tmp > 0) {
            int digit = tmp % 10;
            tmp /= 10;

            if (prev_digit == -1) {
                prev_digit = digit;
                run_len   = 1;
                continue;
            }
            if (digit == prev_digit) {
                ++run_len;
                continue;
            }

            /* finalize previous run */
            seq_t pair_value = (seq_t)run_len * 10 + (seq_t)prev_digit;
            int pair_len = digit_len((seq_t)run_len) + 1;
            cur = cur * pow10((idx_t)pair_len) + pair_value;

            prev_digit = digit;
            run_len    = 1;
        }

        /* finalize last run if any */
        if (prev_digit != -1) {
            seq_t pair_value = (seq_t)run_len * 10 + (seq_t)prev_digit;
            int pair_len = digit_len((seq_t)run_len) + 1;
            cur = cur * pow10((idx_t)pair_len) + pair_value;
        }

        /* ----------------------------------------------------------------
           Test the description for primality; if prime, add p to term cache. */
        if (is_prime_big(cur)) {
            if (term_cnt == term_cap) {
                term_cap *= 2;
                term_cache = (seq_t *)realloc(term_cache, term_cap * sizeof(seq_t));
            }
            term_cache[term_cnt++] = (seq_t)p;
        }

        /* advance to the next candidate for prime generation           */
        next_candidate = (p == 2) ? 3 : p + 2;
    }

    return term_cache[n - 1];
}
