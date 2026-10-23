/*
 * A188536  Potential magic constants of 7 X 7 magic squares composed of
 *          consecutive primes.
 * https://oeis.org/A188536
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (sum of 49 consecutive primes) / 7 for the n‑th block of
 *            49 primes whose sum is a multiple of 7
 * Range:     u8 none, u16 n <= 24, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Prime generation ----------------------------------------------------
 * A simple sieve up to a fixed limit (≈1.6 M) which produces about
 * 120 k primes – enough for all n ≤ 10 000.
 */

static bool primes_built      = false;
static uint64_t *prime_arr    = NULL;
static size_t   prime_count   = 0;

static void build_primes(void)
{
    if (primes_built) return;

    const size_t LIMIT       = 1600000;   /* limit for the sieve   */
    const size_t EST_PRIME_CNT = 200000;  /* upper bound estimate */

    bool *is_comp = (bool *)calloc(LIMIT + 1, sizeof(bool));
    if (!is_comp) exit(EXIT_FAILURE);

    prime_arr     = (uint64_t *)malloc(EST_PRIME_CNT * sizeof(uint64_t));
    if (!prime_arr) exit(EXIT_FAILURE);
    prime_count   = 0;

    for (size_t i = 2; i <= LIMIT; ++i) {
        if (!is_comp[i]) {
            if (prime_count < EST_PRIME_CNT)
                prime_arr[prime_count++] = (uint64_t)i;
            if (i * i <= LIMIT) {
                for (size_t j = i * i; j <= LIMIT; j += i)
                    is_comp[j] = true;
            }
        }
    }

    free(is_comp);
    primes_built = true;
}

/* Storage for computed terms -----------------------------------------
 * The array `terms` holds the already produced values of a(n).
 */

static seq_t *terms    = NULL;
static size_t terms_size = 0;
static size_t terms_cap  = 0;

static void grow_terms(size_t new_cap)
{
    if (new_cap <= terms_cap) return;
    size_t new_size = terms_cap ? terms_cap * 2 : 128;
    if (new_size < new_cap) new_size = new_cap;
    seq_t *new_arr = (seq_t *)realloc(terms, new_size * sizeof(seq_t));
    if (!new_arr) exit(EXIT_FAILURE);
    terms = new_arr;
    terms_cap = new_size;
}

/* Main sequence function ---------------------------------------------
 * Uses a sliding window of 49 consecutive primes.  Whenever the sum
 * of the window is divisible by 7, the sum/7 is stored as the next term.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for n=0, but harmless */

    build_primes();

    /* Ensure enough terms are available */
    if ((size_t)n > terms_size) {
        /* Sliding window state: static so it persists between calls */
        static idx_t  start_idx       = 0;   /* start index of current block */
        static seq_t  current_sum     = 0;   /* sum of primes[start_idx..+48] */
        static bool   initialized     = false;

        /* Initialize window on the first invocation */
        if (!initialized) {
            current_sum = 0;
            for (idx_t i = 0; i < 49; ++i)
                current_sum += (seq_t)prime_arr[i];
            initialized = true;
        }

        /* Generate terms until we have at least n of them */
        while ((size_t)n > terms_size) {
            if ((current_sum % 7) == 0) {
                grow_terms(terms_size + 1);
                terms[terms_size++] = current_sum / 7;
            }

            /* Move window one prime ahead */
            if ((size_t)start_idx + 49 >= prime_count) {
                /* Should not occur for n ≤ 10 000 with our sieve limit */
                break;
            }
            current_sum += (seq_t)prime_arr[start_idx + 49];
            current_sum -= (seq_t)prime_arr[start_idx];
            ++start_idx;
        }
    }

    return terms[(size_t)n - 1];
}
