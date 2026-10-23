/*
 * A107133  Primes of the form 4x^2 + 7y^2.
 * https://oeis.org/A107133
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   primes of the form 4x² + 7y² (no closed formula, computed by * a
 *            sieve plus a congruence test)
 * Range:     u8 n <= 15, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Parameters ----------------------------------------------------------- */
static const uint64_t SIEVE_LIMIT = 5000000ULL;   /* covers >10000 terms */

/* Global storage ------------------------------------------------------- */
static bool initialized = false;

static uint8_t  *is_composite = NULL;          /* is_composite[i] == 1 → composite */
static uint32_t *prime_list   = NULL;          /* list of all primes up to SIEVE_LIMIT */
static uint32_t  prime_count  = 0;

static seq_t *form_primes = NULL;              /* primes of the form 4x²+7y² */
static uint32_t form_count = 0;

/* --------------------------------------------------------------- */
/* Pre‑compute the sieve, prime list, and the desired primes.       */
static void precompute(void)
{
    if (initialized) return;

    /* 1. Sieve of Eratosthenes ------------------------------------ */
    is_composite = (uint8_t*)calloc(SIEVE_LIMIT + 1, sizeof(uint8_t));
    is_composite[0] = is_composite[1] = 1;
    for (uint32_t i = 2; (uint64_t)i * i <= SIEVE_LIMIT; ++i) {
        if (!is_composite[i]) {
            for (uint32_t j = i * i; j <= SIEVE_LIMIT; j += i) {
                is_composite[j] = 1;
            }
        }
    }

    /* 2. Build prime list ---------------------------------------- */
    /* Rough upper bound on number of primes (n/log n).               */
    uint32_t prime_capacity = (uint32_t)(SIEVE_LIMIT / 5 + 100);
    prime_list = (uint32_t*)malloc(prime_capacity * sizeof(uint32_t));
    for (uint32_t i = 2; i <= SIEVE_LIMIT; ++i) {
        if (!is_composite[i]) {
            if (prime_count >= prime_capacity) { /* resize if needed */
                prime_capacity *= 2;
                prime_list = (uint32_t*)realloc(
                    prime_list, prime_capacity * sizeof(uint32_t));
            }
            prime_list[prime_count++] = i;
        }
    }

    /* 3. Extract primes of the form 4x²+7y² ------------------------ */
    uint32_t form_capacity = prime_count / 3 + 10;
    form_primes = (seq_t*)malloc(form_capacity * sizeof(seq_t));
    for (uint32_t i = 0; i < prime_count; ++i) {
        uint32_t p = prime_list[i];
        if (p == 7 || p % 28 == 11 || p % 28 == 15 || p % 28 == 23) {
            if (form_count >= form_capacity) {   /* unlikely but safe */
                form_capacity *= 2;
                form_primes = (seq_t*)realloc(
                    form_primes, form_capacity * sizeof(seq_t));
            }
            form_primes[form_count++] = (seq_t)p;
        }
    }

    initialized = true;
}

/* ------------------------------------------------------------------- */
/* Return the n‑th prime of the form 4x² + 7y² (1‑based index). */
/* The function is safe for any n ≥ 1 that fits within the pre‑computed
 * range; larger n will return 0.                                          */
seq_t a(idx_t n)
{
    precompute();

    if ((uint64_t)n == 0) return 0;         /* undefined, but return 0 */

    if ((uint64_t)n <= (uint64_t)form_count) {
        return form_primes[n - 1];
    }

    /* If n is larger than the pre‑computed range, we could continue
     * generating more primes using trial division, but the problem
     * statement only requires correctness for the first ~10,000 terms.
     * Therefore we simply return 0 for larger indices. */
    return 0;
}
