/*
 * A225677  Primes of the form p - (product of digits of p), where p is a
 *          prime.
 * https://oeis.org/A225677
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   primes of the form p - product_of_digits(p) where p is prime
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   cmp_seq_t, digit_product, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function builds the list once by sieving primes up to a fixed
   limit and then searching for the required values.  Subsequent calls
   return elements of that list. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/digit_product.h"
#include "../utils/isPrimeGeneric.h"
#include "../utils/cmp_seq_t.h"

/* Maximum prime to sieve.  This limit guarantees > 10 000 terms for all
   test cases up to n ≈ 10 000. */
#define SIEVE_LIMIT 10000000ULL

static seq_t *seq = NULL;       /* sorted unique sequence values     */
static size_t seq_len = 0;      /* number of elements in seq        */
static size_t seq_cap = 0;      /* capacity of seq array            */

static uint64_t *prime_list = NULL;  /* list of primes up to the limit   */
static size_t prime_count = 0;       /* number of primes                  */

static void init_sequence(void)
{
    /* --- 1.  Sieve of Eratosthenes ----------------------------------- */
    size_t N = SIEVE_LIMIT + 1;          /* array size (index up to SIEVE_LIMIT) */
    bool *is_composite = (bool *)calloc(N, 1);   /* zero‑init */
    if (!is_composite) exit(EXIT_FAILURE);

    for (size_t i = 2; i * i < N; ++i) {
        if (!is_composite[i]) {
            for (size_t j = i * i; j < N; j += i)
                is_composite[j] = 1;
        }
    }

    /* --- 2.  Extract prime list --------------------------------------- */
    size_t cnt = 0;
    for (size_t i = 2; i < N; ++i)
        if (!is_composite[i]) ++cnt;

    prime_list = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!prime_list) exit(EXIT_FAILURE);
    size_t idx = 0;
    for (size_t i = 2; i < N; ++i)
        if (!is_composite[i]) prime_list[idx++] = (uint64_t)i;
    prime_count = cnt;
    free(is_composite);

    /* --- 3.  Generate candidate sequence values ------------------------ */
    seq_t *tmp = (seq_t *)malloc(cnt * sizeof(seq_t));  /* temporary array */
    if (!tmp) exit(EXIT_FAILURE);
    size_t tmp_len = 0;

    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = prime_list[i];
        seq_t prod = digit_product((seq_t)p);  /* product of digits of p   */
        seq_t q = (seq_t)p - prod;             /* candidate value          */
        if (isPrimeGeneric(q))                 /* primality check          */
            tmp[tmp_len++] = q;
    }

    /* --- 4.  Sort and deduplicate ------------------------------------- */
    if (tmp_len > 0)
        qsort(tmp, tmp_len, sizeof(seq_t), cmp_seq_t);

    seq_cap = tmp_len;
    seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
    if (!seq) exit(EXIT_FAILURE);
    seq_len = 0;
    for (size_t i = 0; i < tmp_len; ++i) {
        if (i == 0 || tmp[i] != tmp[i - 1]) {
            seq[seq_len++] = tmp[i];
        }
    }

    free(tmp);
    free(prime_list);
    prime_list = NULL;
    prime_count = 0;
}

/* Public function required by the framework */
seq_t a(idx_t n)
{
    static bool initialized = false;
    if (!initialized) {
        init_sequence();
        initialized = true;
    }
    if (n == 0 || (size_t)n > seq_len)
        return (seq_t)0;
    return seq[(size_t)n - 1];
}
