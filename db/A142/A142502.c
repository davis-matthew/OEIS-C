/*
 * A142502  Primes congruent to 43 mod 51.
 * https://oeis.org/A142502
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 202, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes congruent to 43 mod 51
 * a(n) is the n‑th prime p with p ≡ 43 (mod 51)
 * the function builds the sequence lazily and caches results
 * for arbitrary order of calls.
 */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

/* --- Static cache ----------------------------------------------------- */

static uint32_t *prime_list = NULL;   /* all computed primes   */
static size_t prime_cnt   = 0;
static size_t prime_cap   = 0;

static uint32_t *term_list = NULL;    /* primes ≡ 43 (mod 51)  */
static size_t term_cnt   = 0;
static size_t term_cap   = 0;

/* next integer to test for primality                         */
static uint32_t next_check = 2;       /* start at the first prime */

/* ---- Helper functions --------------------------------------------- */

/* Grow the array that holds primes.  Capacity is doubled when needed. */
static void grow_prime_list(void)
{
    if (prime_cnt >= prime_cap) {
        size_t new_cap = prime_cap ? prime_cap * 2U : 64U;
        uint32_t *tmp = (uint32_t*)realloc(prime_list, new_cap * sizeof(uint32_t));
        if (!tmp) return; /* allocation failure – leave state unchanged */
        prime_list = tmp;
        prime_cap  = new_cap;
    }
}

/* Grow the array that holds the required terms. */
static void grow_term_list(void)
{
    if (term_cnt >= term_cap) {
        size_t new_cap = term_cap ? term_cap * 2U : 64U;
        uint32_t *tmp = (uint32_t*)realloc(term_list, new_cap * sizeof(uint32_t));
        if (!tmp) return;
        term_list = tmp;
        term_cap  = new_cap;
    }
}

/* Find the next prime > last found prime and store it in prime_list. */
static void generate_next_prime(void)
{
    while (1) {
        uint32_t candidate = next_check++;
        if (candidate == 2) {
            /* 2 is prime */
            grow_prime_list();
            prime_list[prime_cnt++] = candidate;
            break;
        }
        if ((candidate & 1) == 0) continue;               /* skip even */

        uint32_t limit = (uint32_t)sqrt((double)candidate);
        int is_prime = 1;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint32_t p = prime_list[i];
            if (p > limit) break;
            if ((candidate % p) == 0) { is_prime = 0; break; }
        }
        if (is_prime) {
            grow_prime_list();
            prime_list[prime_cnt++] = candidate;
            break;
        }
    }
}

/* Generate terms until we have at least n terms. */
static void ensure_terms(idx_t n)
{
    while (term_cnt < (size_t)n) {
        /* obtain the next prime */
        generate_next_prime();

        /* test the congruence condition */
        uint32_t p = prime_list[prime_cnt - 1];
        if (p % 51U == 43U) {
            grow_term_list();
            term_list[term_cnt++] = p;
        }
    }
}

/* --------------------------------------------------------------------- */

/* External function required by the task */
seq_t a(idx_t n)
{
    if (n < 1U) return (seq_t)0;     /* undefined for offset 0 */

    ensure_terms(n);

    if ((size_t)n <= term_cnt)
        return (seq_t)term_list[n - 1];
    return (seq_t)0;                 /* safety value if failure occurs */
}
