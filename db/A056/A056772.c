/*
 * A056772  Numbers k such that phi(k+4) = phi(k) + 4, where phi(k) =
 *          A000010(k) is Euler's totient function.
 * https://oeis.org/A056772
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 1042, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses a totient sieve to collect values k satisfying φ(k+4)=φ(k)+4. */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/grow_seq.h"

/* Limits chosen to generate at least 10 000 terms (≈ 14 000 prime‑p‑4 pairs). */
static const idx_t LIMIT = 5000000;

static uint64_t *phi_arr       = NULL;   /* φ values */
static uint8_t  *is_comp_arr   = NULL;   /* composite markers */
static idx_t   *prime_arr      = NULL;   /* list of primes */
static size_t   prime_cnt      = 0;

static seq_t *terms_cache      = NULL;   /* discovered sequence terms */
static size_t terms_len        = 0;
static size_t terms_cap        = 0;
static idx_t  last_check       = 1;

/* ------------------------------------------------------------------ */
/* Build the linear totient sieve up to LIMIT. */
static void init_sieve(void)
{
    phi_arr     = (uint64_t*)malloc((LIMIT + 1) * sizeof(uint64_t));
    is_comp_arr = (uint8_t*)calloc(LIMIT + 1, sizeof(uint8_t));
    prime_arr   = (idx_t*)malloc(LIMIT * sizeof(idx_t));
    if (!phi_arr || !is_comp_arr || !prime_arr) abort(); /* fatal error */

    phi_arr[1] = 1;

    for (idx_t i = 2; i <= LIMIT; ++i) {
        if (!is_comp_arr[i]) {                /* i is prime */
            prime_arr[prime_cnt++] = i;
            phi_arr[i] = i - 1;
        }
        for (size_t j = 0; j < prime_cnt; ++j) {
            idx_t p  = prime_arr[j];
            idx_t ip = i * p;
            if (ip > LIMIT) break;
            is_comp_arr[ip] = 1;
            if (i % p == 0) {
                phi_arr[ip] = phi_arr[i] * p;
                break;
            } else {
                phi_arr[ip] = phi_arr[i] * (p - 1);
            }
        }
    }
}

/* ------------------------------------------------------------------ */
/* Main sequence accessor. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined, keep safety */

    /* Lazy initialisation */
    if (!phi_arr) init_sieve();

    /* Generate terms until we have at least n */
    while (terms_len < n) {
        for (idx_t k = last_check; k + 4 <= LIMIT; ++k) {
            /* quick check using pre‑computed totients */
            if (phi_arr[k + 4] - phi_arr[k] == 4) {
                grow_seq(&terms_cache, &terms_cap, terms_len + 1);
                terms_cache[terms_len++] = (seq_t)k;
                if (terms_len >= n) {
                    last_check = k + 1;
                    break;
                }
            }
        }
        /* If we reached the sieve limit with insufficient terms, extend */
        if (terms_len < n) {
            /* Allocate a larger sieve */
            idx_t new_limit = LIMIT * 2;
            free(phi_arr); free(is_comp_arr); free(prime_arr);
            /* Update LIMIT would need redefinition, so we use local variable */
            uint64_t *new_phi      = (uint64_t*)malloc((new_limit + 1) * sizeof(uint64_t));
            uint8_t  *new_comp     = (uint8_t*)calloc(new_limit + 1, sizeof(uint8_t));
            idx_t    *new_primes   = (idx_t*)malloc(new_limit * sizeof(idx_t));
            if (!new_phi || !new_comp || !new_primes) abort();

            /* Linear sieve for the larger range */
            new_phi[1] = 1;
            size_t new_prime_cnt = 0;
            for (idx_t i = 2; i <= new_limit; ++i) {
                if (!new_comp[i]) {
                    new_primes[new_prime_cnt++] = i;
                    new_phi[i] = i - 1;
                }
                for (size_t j = 0; j < new_prime_cnt; ++j) {
                    idx_t p  = new_primes[j];
                    idx_t ip = i * p;
                    if (ip > new_limit) break;
                    new_comp[ip] = 1;
                    if (i % p == 0) {
                        new_phi[ip] = new_phi[i] * p;
                        break;
                    } else {
                        new_phi[ip] = new_phi[i] * (p - 1);
                    }
                }
            }

            /* Replace old arrays */
            free(phi_arr); free(is_comp_arr); free(prime_arr);
            phi_arr     = new_phi;
            is_comp_arr = new_comp;
            prime_arr   = new_primes;
            prime_cnt   = new_prime_cnt;
            /* Reset state to re‑scan from beginning for remaining terms */
            last_check = 1;
        }
    }

    return terms_cache[n - 1];
}
