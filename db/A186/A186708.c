/*
 * A186708  Number of quadratic residues (mod p) in the interval [1,2k+1], for
 *          primes p=4k+3.
 * https://oeis.org/A186708
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   count_qr
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = number of quadratic residues modulo the n‑th prime
 *   p = 4k + 3 in the interval [1,2k+1].
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/count_qr.h"

/* Grow an array of uint32_t when more capacity is needed. */
static void grow_uint32(uint32_t **arr, size_t *cap, size_t new_cap)
{
    if (*cap >= new_cap) return;
    *arr = (uint32_t *)realloc(*arr, new_cap * sizeof(uint32_t));
    *cap  = new_cap;
}

seq_t a(idx_t n)
{
    /* Static state for prime generation. */
    static bool            initialized = false;
    static uint32_t *      prime_list  = NULL;   /* all primes found so far      */
    static size_t          prime_cnt   = 0, prime_cap   = 0;
    static uint32_t *      prime4k3    = NULL;   /* primes ≡ 3 (mod 4)           */
    static size_t          prime4k3_cnt = 0, prime4k3_cap = 0;
    static uint32_t        last_candidate = 1;    /* next odd number to test      */

    if (!initialized) {
        /* prime_list */
        prime_cap   = 64;
        prime_list  = (uint32_t *)malloc(prime_cap * sizeof(uint32_t));
        prime_list[0] = 2;
        prime_cnt    = 1;

        /* prime4k3 */
        prime4k3_cap = 64;
        prime4k3     = (uint32_t *)malloc(prime4k3_cap * sizeof(uint32_t));
        prime4k3_cnt = 0;

        last_candidate = 3;          /* start testing from 3 */
        initialized = true;
    }

    /* Grow prime lists until we have n primes of the form 4k+3. */
    while (prime4k3_cnt < (size_t)n) {
        uint32_t cand = last_candidate;
        bool      is_prime = true;

        /* trial division by known primes up to sqrt(cand) */
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint32_t p = prime_list[i];
            if ((uint64_t)p * p > cand) break;
            if (cand % p == 0) { is_prime = false; break; }
        }

        if (is_prime) {
            /* add to overall list */
            if (prime_cnt == prime_cap) {
                grow_uint32(&prime_list, &prime_cap, prime_cap << 1);
            }
            prime_list[prime_cnt++] = cand;

            /* add to 4k+3 list if applicable */
            if ((cand & 3U) == 3U) {
                if (prime4k3_cnt == prime4k3_cap) {
                    grow_uint32(&prime4k3, &prime4k3_cap, prime4k3_cap << 1);
                }
                prime4k3[prime4k3_cnt++] = cand;
            }
        }
        last_candidate += 2;   /* only test odd numbers */
    }

    uint32_t p = prime4k3[n - 1];
    return count_qr(p);
}
