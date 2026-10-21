/*
 * A064233  Numbers that are not the sum of a prime number and a nonzero
 *          square.
 * https://oeis.org/A064233
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 195, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A064233: Numbers that are not the sum of a prime number and a non‑zero square.
   This implementation builds the sequence on demand by sieving primes up to a dynamically
   increasing limit and marking all numbers that can be written as p + s² (p prime, s≥1).
   It then collects the remaining integers as the required sequence.
*/

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

/*  seq_t and idx_t are defined by the build system.  No other declarations.  */

static seq_t *seq_arr  = NULL;   /* array of sequence terms */
static size_t seq_len  = 0;      /* number of terms stored */
static size_t seq_cap  = 0;      /* capacity of seq_arr */

static uint8_t *excl_arr = NULL; /* array marking numbers that are prime+square */
static size_t excl_cap  = 0;      /* capacity of excl_arr */

static uint32_t *prime_arr = NULL; /* primes up to current limit */
static size_t prime_len   = 0;
static size_t prime_cap   = 0;

static idx_t current_limit = 0;   /* max integer processed so far */

/* Helper: resize an array of seq_t elements */
static void seq_resize(size_t new_cap) {
    if (new_cap > seq_cap) {
        seq_cap = new_cap;
        seq_arr = (seq_t *)realloc(seq_arr, seq_cap * sizeof(seq_t));
    }
}

/* Helper: resize an array of uint32_t primes */
static void primes_resize(size_t new_cap) {
    if (new_cap > prime_cap) {
        prime_cap = new_cap;
        prime_arr = (uint32_t *)realloc(prime_arr, prime_cap * sizeof(uint32_t));
    }
}

/* Generate all primes up to 'limit' and mark numbers that are prime + non‑zero square.  */
static void generate_up_to(idx_t limit) {
    /* Allocate/resize excluded array */
    size_t new_excl_cap = (size_t)limit + 1;
    if (new_excl_cap > excl_cap) {
        excl_cap = new_excl_cap;
        excl_arr = (uint8_t *)realloc(excl_arr, excl_cap * sizeof(uint8_t));
    }
    for (size_t i = 0; i <= limit; ++i) excl_arr[i] = 0;

    /* Sieve of Eratosthenes to obtain primes up to limit */
    uint8_t *is_prime = (uint8_t *)malloc((size_t)limit + 1);
    for (size_t i = 0; i <= limit; ++i) is_prime[i] = 1;
    if (limit >= 0) is_prime[0] = 0;
    if (limit >= 1) is_prime[1] = 0;

    prime_len = 0;
    for (idx_t p = 2; p <= limit; ++p) {
        if (is_prime[p]) {
            /* store the prime */
            if (prime_len == prime_cap) primes_resize(prime_cap ? prime_cap * 2 : 16);
            prime_arr[prime_len++] = (uint32_t)p;

            /* mark multiples */
            if ((uint64_t)p * (uint64_t)p <= (uint64_t)limit) {
                for (uint64_t m = (uint64_t)p * (uint64_t)p; m <= (uint64_t)limit; m += (uint64_t)p)
                    is_prime[(size_t)m] = 0;
            }
        }
    }

    /* Mark every number that can be written as p + s² (s ≥ 1) */
    for (size_t pi = 0; pi < prime_len; ++pi) {
        uint32_t p = prime_arr[pi];
        for (idx_t s = 1;; ++s) {
            idx_t sq = s * s;
            if (p + sq > limit) break;
            excl_arr[p + sq] = 1;
        }
    }

    free(is_prime);

    /* Build the sequence array up to this limit */
    seq_resize(limit / 10 + 20);  /* rough capacity estimate */
    seq_len = 0;
    for (idx_t i = 1; i <= limit; ++i) {
        if (!excl_arr[i]) {
            if (seq_len == seq_cap) seq_resize(seq_cap * 2);
            seq_arr[seq_len++] = (seq_t)i;
        }
    }

    current_limit = limit;
}

/* Ensure we have at least 'n' terms in the sequence. */
static void ensure_terms(idx_t n) {
    if (seq_len >= n) return;

    idx_t limit = current_limit ? current_limit * 2 : 50000;
    while (seq_len < n) {
        generate_up_to(limit);
        if (seq_len >= n) break;
        limit *= 2;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* undefined for offset 0 */

    ensure_terms(n);
    return seq_arr[n - 1];
}
