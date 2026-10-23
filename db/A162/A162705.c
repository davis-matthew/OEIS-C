/*
 * A162705  Numbers that are the sum of two reversed consecutive primes in
 *          more than one way.
 * https://oeis.org/A162705
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 115, u64 n <= 115, u128 n <= 115,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: Enumerate sums of reversed consecutive primes,
   keep those sums that appear at least twice, and return the nth such
   sum (with OEIS offset 1). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/* Helper: reverse the decimal representation of a 64‑bit unsigned int */
/* ------------------------------------------------------------------ */
static inline uint64_t rev_uint64(uint64_t x)
{
    uint64_t r = 0;
    while (x != 0) {
        r = r * 10 + (x % 10);
        x /= 10;
    }
    return r;
}

/* ------------------------------------------------------------------ */
/* Comparator for qsort over seq_t (works for all seq_t types)         */
/* ------------------------------------------------------------------ */
static int cmp_seq(const void *a, const void *b)
{
    const seq_t *sa = (const seq_t *)a;
    const seq_t *sb = (const seq_t *)b;
    if (*sa < *sb) return -1;
    if (*sa > *sb) return 1;
    return 0;
}

/* ------------------------------------------------------------------ */
/* Sieve of Eratosthenes returning all primes ≤ limit                  */
/* In C++ the cast for malloc is required; in C it is harmless.        */
/* ------------------------------------------------------------------ */
static void build_primes_upto(uint64_t limit, uint64_t **primes_out,
                              size_t *cnt_out)
{
    uint8_t *sieve = (uint8_t *)malloc((size_t)limit + 1);
    if (!sieve) {
        *primes_out = NULL;
        *cnt_out = 0;
        return;
    }

    for (uint64_t i = 0; i <= limit; ++i) sieve[i] = 1;
    sieve[0] = sieve[1] = 0;

    for (uint64_t i = 2; i * i <= limit; ++i)
        if (sieve[i])
            for (uint64_t j = i * i; j <= limit; j += i)
                sieve[j] = 0;

    size_t cnt = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (sieve[i]) ++cnt;

    uint64_t *primes = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!primes) {
        free(sieve);
        *primes_out = NULL;
        *cnt_out = 0;
        return;
    }

    size_t pos = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (sieve[i]) primes[pos++] = i;

    free(sieve);
    *primes_out = primes;
    *cnt_out = cnt;
}

/* ------------------------------------------------------------------ */
/* Static memoisation state                                         */
/* ------------------------------------------------------------------ */
static uint64_t prime_limit = 2000000ULL;   /* start large enough */
static uint64_t *prime_list = NULL;
static size_t     prime_cnt   = 0;
static seq_t *result_seq  = NULL;
static size_t result_cnt  = 0;
static idx_t max_n_needed = 0;

/* ------------------------------------------------------------------ */
/* Extend precomputation until we have at least n_needed terms         */
/* ------------------------------------------------------------------ */
static void recompute_up_to(idx_t n_needed)
{
    if (n_needed <= result_cnt) return;

    while (result_cnt < (size_t)n_needed) {
        /* Build new prime list */
        free(prime_list);
        build_primes_upto(prime_limit, &prime_list, &prime_cnt);
        if (prime_cnt < 2) {
            result_cnt = 0;
            free(result_seq);
            result_seq = NULL;
            break;
        }

        size_t sum_cnt = prime_cnt - 1;
        seq_t *sums = (seq_t *)malloc(sum_cnt * sizeof(seq_t));
        if (!sums) break;   /* allocation failure – unlikely for limits we use */
        for (size_t i = 0; i < sum_cnt; ++i) {
            seq_t r1 = (seq_t)rev_uint64(prime_list[i]);
            seq_t r2 = (seq_t)rev_uint64(prime_list[i + 1]);
            sums[i] = r1 + r2;
        }

        /* Sort sums to detect duplicates easily */
        qsort(sums, sum_cnt, sizeof(seq_t), cmp_seq);

        /* Collect distinct sums that appear at least twice */
        seq_t *tmp = NULL;
        size_t tmpc = 0, tmpsize = 0;
        for (size_t i = 0; i < sum_cnt; ) {
            size_t j = i + 1;
            while (j < sum_cnt && sums[i] == sums[j]) ++j;
            if (j - i >= 2) {                   /* at least two occurrences */
                if (tmpc == tmpsize) {
                    tmpsize = tmpsize ? tmpsize * 2 : 8;
                    tmp = (seq_t *)realloc(tmp, tmpsize * sizeof(seq_t));
                }
                tmp[tmpc++] = sums[i];
            }
            i = j;
        }
        free(sums);
        free(result_seq);
        result_seq = tmp;
        result_cnt = tmpc;

        /* If still insufficient, increase the prime upper bound*/
        if (result_cnt < (size_t)n_needed)
            prime_limit *= 2;
    }
}

/* ------------------------------------------------------------------ */
/* Public function required by the framework                         */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* below offset – undefined */

    if ((idx_t)max_n_needed < n) {
        max_n_needed = n;
        recompute_up_to(n);
    }

    /* The precomputation guarantees result_cnt >= n */
    return result_seq[n - 1];
}
