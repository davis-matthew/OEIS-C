/*
 * A066876  Numbers n such that prime(k+2) + prime(k-2) = 2*prime(k).
 * https://oeis.org/A066876
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A066876:
   a(n) = the n-th integer k (k ≥ 5) for which
   prime(k+2) + prime(k-2) = 2 * prime(k).
   Prime indices use the usual 1‑based indexing
   (prime(1)=2, prime(2)=3, ...).  The sequence starts
   with a(1)=5.  The implementation builds a sieve of
   primes up to a fixed limit, then generates the
   list of qualifying indices on first call.
*/

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

/* Static initialization flag */
static bool seq_initialized = false;
static uint64_t *prime_list = NULL;  /* array of primes (1‑based indices map to prime_list[0] = prime 1) */
static size_t prime_count = 0;
static seq_t *term_list = NULL;      /* array of qualifying k indices */
static size_t term_count = 0;
static size_t term_cap   = 0;

/* Limit of primes to generate (adjust if needed) */
#define PRIME_LIMIT 5000000ULL

/* Helper: generate all primes ≤ limit using sieve of Eratosthenes */
static void generate_primes_upto(idx_t limit, uint64_t **primes_out, size_t *cnt_out)
{
    /* Count of odd numbers up to limit (excluding 2) */
    size_t odd_cnt = (limit >= 3) ? ((limit - 3) / 2 + 1) : 0;
    bool *is_composite = (bool *)calloc(odd_cnt, sizeof(bool));
    if (!is_composite) exit(1);  /* allocation failure */

    idx_t sqrt_limit = (idx_t)sqrtl((long double)limit);
    for (idx_t i = 3; i <= sqrt_limit; i += 2) {
        if (!is_composite[(i - 3) / 2]) {
            for (idx_t j = i * i; j <= limit; j += 2 * i)
                is_composite[(j - 3) / 2] = true;
        }
    }

    /* Count primes */
    size_t count = (limit >= 2) ? 1 : 0;  /* prime 2 */
    for (idx_t i = 3; i <= limit; i += 2)
        if (!is_composite[(i - 3) / 2]) count++;

    uint64_t *primes = (uint64_t *)malloc(count * sizeof(uint64_t));
    if (!primes) exit(1);

    size_t pos = 0;
    if (limit >= 2) primes[pos++] = 2;
    for (idx_t i = 3; i <= limit; i += 2)
        if (!is_composite[(i - 3) / 2]) primes[pos++] = (uint64_t)i;

    free(is_composite);
    *primes_out = primes;
    *cnt_out    = count;
}

/* Helper: append a value to the term list, expanding capacity if needed */
static void append_term(seq_t value)
{
    if (term_count >= term_cap) {
        term_cap = (term_cap == 0) ? 64 : term_cap * 2;
        term_list = (seq_t *)realloc(term_list, term_cap * sizeof(seq_t));
        if (!term_list) exit(1);
    }
    term_list[term_count++] = value;
}

/* Generate the qualifying k indices up to the end of prime list */
static void compute_terms(void)
{
    for (idx_t k = 5; k + 2 <= prime_count; ++k) {
        /* prime indices are 1‑based, array indices 0‑based:
           prime(k)     -> prime_list[k-1]
           prime(k-2)   -> prime_list[k-3]
           prime(k+2)   -> prime_list[k+1]   */
        uint64_t left  = prime_list[k - 3];
        uint64_t middle= prime_list[k - 1];
        uint64_t right = prime_list[k + 1];
        if (left + right == 2 * middle)
            append_term((seq_t)k);
    }
}

/* Public sequence function */
seq_t a(idx_t n)
{
    /* Lazy initialization */
    if (!seq_initialized) {
        seq_initialized = true;
        generate_primes_upto(PRIME_LIMIT, &prime_list, &prime_count);
        term_cap   = 0;
        term_list  = NULL;
        term_count = 0;
        compute_terms();
    }

    if (n < 1 || (size_t)n > term_count)
        return 0;                 /* undefined for out of range queries */
    return term_list[n - 1];
}
