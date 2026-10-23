/*
 * A124519  Numbers k such that 12*k - 1 and 12*k + 1 are twin primes.
 * https://oeis.org/A124519
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 38, u16 n <= 3366, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th integer k such that 12*k‑1 and 12*k+1 are prime. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/* ---------- static state ---------- */
static const idx_t LIMIT = 20000000;          /* sieve upper bound (covers 12*k+1) */
static seq_t *valid_k = NULL;                 /* array of qualifying k values   */
static size_t valid_k_used = 0;               /* number of stored k values      */
static size_t valid_k_cap  = 0;               /* allocated capacity             */
static bool    initialized = false;           /* one‑time initialization flag   */

/* ---------- one‑time initialization ---------- */
static void init_sequence(void)
{
    if (initialized) return;

    /* --- sieve primes up to LIMIT --- */
    uint8_t *is_prime = (uint8_t *)malloc((size_t)(LIMIT + 1));
    if (!is_prime) abort();
    memset(is_prime, 1, (size_t)(LIMIT + 1));
    is_prime[0] = is_prime[1] = 0;
    for (idx_t i = 2; i * i <= LIMIT; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= LIMIT; j += i)
                is_prime[j] = 0;
        }
    }

    /* --- collect qualifying k values --- */
    idx_t max_k = (LIMIT - 1) / 12;           /* largest k with 12*k+1 <= LIMIT */
    valid_k_cap = max_k / 10 + 10;            /* initial capacity estimate           */
    valid_k = (seq_t *)malloc(valid_k_cap * sizeof(seq_t));
    if (!valid_k) abort();

    for (idx_t k = 1; k <= max_k; ++k) {
        if (is_prime[12 * k - 1] && is_prime[12 * k + 1]) {
            if (valid_k_used == valid_k_cap) {
                valid_k_cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(valid_k, valid_k_cap * sizeof(seq_t));
                if (!tmp) abort();
                valid_k = tmp;
            }
            valid_k[valid_k_used++] = (seq_t)k;
        }
    }

    free(is_prime);
    initialized = true;
}

/* ---------- sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                   /* undefined for index 0, safe fallback */
    init_sequence();                         /* ensure sieve and table are ready */
    if (n > (idx_t)valid_k_used) return 0;   /* out of bounds – should not happen */
    return valid_k[n - 1];
}
