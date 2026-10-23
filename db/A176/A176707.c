/*
 * A176707  Sum of digits of all distinct prime factors of n-th semiprime.
 * https://oeis.org/A176707
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 523, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_spf, digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A176707: for the n‑th semiprime, sum the decimal digits of all
   distinct prime factors.  The function generates the required semiprimes
   on demand and caches them for future calls. */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "../utils/build_spf.h"   /* build_spf(idx_t limit, idx_t **spf_out) */
#include "../utils/digit_sum.h"   /* digit_sum(seq_t v) */

/* Cache state */
static idx_t max_needed = 0;                /* largest n that has been generated */
static uint64_t *semiprimes = NULL;         /* array of discovered semiprimes */
static size_t semiprime_count = 0;          /* number of semiprimes stored */
static idx_t *spf_table = NULL;             /* smallest prime factor array */

/* Check if num is a semiprime using the pre‑computed SPF table */
static bool is_semiprime(idx_t num)
{
    idx_t cnt = 0;
    while (num > 1) {
        idx_t p = spf_table[num];
        if (p == 0)          /* prime larger than spf_limit */
            p = num;
        num /= p;
        ++cnt;
        if (cnt > 2)
            return false;
    }
    return cnt == 2;
}

/* Sum decimal digits of all distinct prime factors of num.
   Uses digit_sum for each distinct prime factor. */
static seq_t sum_distinct_prime_digit_sum(uint64_t num)
{
    seq_t sum = 0;
    seq_t last = 0;            /* to detect repetition */
    while (num > 1) {
        idx_t p = spf_table[num];
        if (p == 0)           /* prime larger than spf_limit */
            p = num;
        if ((seq_t)p != last) {
            sum += digit_sum((seq_t)p);
            last = (seq_t)p;
        }
        while (num % p == 0)
            num /= p;
    }
    return sum;
}

/* Ensure that at least n semiprimes are available in the cache. */
static void ensure_semiprimes(idx_t n)
{
    if (n <= max_needed)
        return;                  /* already have enough data */

    /* Rough upper bound for the n‑th semiprime. For n up to 10000
       the bound 20*n + 100 is comfortably large. */
    idx_t new_limit = n * 20 + 100;
    if (new_limit < 100)
        new_limit = 100;

    /* Build a new SPF table up to new_limit. */
    if (spf_table)
        free(spf_table);
    build_spf(new_limit, &spf_table);
    /* spf_table now holds smallest prime factor for 1..new_limit */

    /* Allocate space for semiprimes. Grow capacity as needed. */
    size_t capacity = (size_t)new_limit * 2;      /* generous start */
    if (semiprimes)
        free(semiprimes);                        /* discard old data */
    semiprimes = (uint64_t *)malloc(capacity * sizeof(uint64_t));
    semiprime_count = 0;

    /* Generate semiprimes up to new_limit. Stop once we have n of them. */
    for (idx_t i = 4; i <= new_limit; ++i) {
        if (is_semiprime(i)) {
            if (semiprime_count >= capacity) {
                capacity *= 2;
                semiprimes = (uint64_t *)realloc(semiprimes,
                                                capacity * sizeof(uint64_t));
            }
            semiprimes[semiprime_count++] = i;
            if (semiprime_count >= n)
                break;  /* enough for the current request */
        }
    }

    max_needed = n;
}

/* Public entry point required by the test harness. */
seq_t a(idx_t n)
{
    if (n == 0)          /* safety for out‑of‑range calls */
        return 0;

    ensure_semiprimes(n);

    uint64_t sem = semiprimes[n - 1];
    return sum_distinct_prime_digit_sum(sem);
}
