/*
 * A176134  Primes p of the form |prime(n+2)^2-prime(n+1)^2-prime(n)^2|,
 *          (absolute values).
 * https://oeis.org/A176134
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 21, u32 n <= 24, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   is_prime_big, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A176134: a(n) = prime of the form |prime(n+2)^2 - prime(n+1)^2 - prime(n)^2|
 * Only the number itself is prime after the absolute value. The sequence is offset 1.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/next_prime.h"
#include "../utils/is_prime_big.h"

static seq_t *seq_values = NULL;     /* array of computed sequence terms */
static size_t seq_cap   = 0;         /* current capacity of seq_values      */
static idx_t seq_len    = 0;         /* number of sequence terms stored */

static uint64_t *prime_arr = NULL;  /* array of first primes */
static size_t prime_cap   = 0;       /* current capacity of prime_arr   */
static idx_t prime_len    = 0;       /* number of primes stored         */

static idx_t next_candidate = 0;     /* index n of the next triple to test */

/* Ensure seq_values has room for 'need' elements */
static void ensure_seq_cap(size_t need)
{
    if (need <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap : 8;
    while (new_cap < need) new_cap <<= 1;
    seq_values = (seq_t *)realloc(seq_values, new_cap * sizeof(seq_t));
    seq_cap    = new_cap;
}

/* Ensure prime_arr has room for 'need' primes */
static void ensure_prime_cap(size_t need)
{
    if (need <= prime_cap) return;
    size_t new_cap = prime_cap ? prime_cap : 8;
    while (new_cap < need) new_cap <<= 1;
    prime_arr = (uint64_t *)realloc(prime_arr, new_cap * sizeof(uint64_t));
    prime_cap = new_cap;
}

/* Initialise the first three primes */
static void init_primes(void)
{
    if (prime_len == 0) {
        ensure_prime_cap(3);
        prime_arr[0] = 2;
        prime_arr[1] = 3;
        prime_arr[2] = 5;
        prime_len    = 3;
    }
}

/* Generate the next prime and store it */
static void generate_next_prime(void)
{
    uint64_t next = next_prime(prime_arr[prime_len - 1] + 1);
    ensure_prime_cap(prime_len + 1);
    prime_arr[prime_len++] = next;
}

/* Compute sequence terms until we have at least 'n' terms */
static void compute_terms_up_to(idx_t n)
{
    init_primes();
    while (seq_len < n) {
        /* Ensure we have enough primes for the current triple */
        while (prime_len < next_candidate + 3) {
            generate_next_prime();
        }

        uint64_t p0 = prime_arr[next_candidate];
        uint64_t p1 = prime_arr[next_candidate + 1];
        uint64_t p2 = prime_arr[next_candidate + 2];

        /* Use 128‑bit arithmetic to avoid overflow */
        signed __int128 diff128 = (signed __int128)p2 * p2
                               - (signed __int128)p1 * p1
                               - (signed __int128)p0 * p0;
        if (diff128 < 0) diff128 = -diff128;

        /* Only store if the resulting value is prime */
        if (is_prime_big((seq_t)diff128)) {
            ensure_seq_cap(seq_len + 1);
            seq_values[seq_len++] = (seq_t)diff128;
        }

        /* Move to the next triple, regardless of whether we stored a value */
        next_candidate++;
    }
}

/* External function required by the framework */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined below offset, harmless */
    if (seq_len < n) compute_terms_up_to(n);
    return seq_values[n - 1];              /* offset 1: a(1) is the first term */
}
