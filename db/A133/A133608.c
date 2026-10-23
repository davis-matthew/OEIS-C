/*
 * A133608  Numbers n such that the sum of digits of n-th semiprime equals sum
 *          of digits of n.
 * https://oeis.org/A133608
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, less
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = the n-th integer k such that
   digit_sum(semiprime(k)) == digit_sum(k)   (A133608)   */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/digit_sum.h"

/* ------------------------------------------------------------------ */
/*  semiprime storage, spf sieve and dynamic growth                   */
/* ------------------------------------------------------------------ */
static seq_t   *sem_list  = NULL;   /* list of semiprime values, 1‑based index */
static idx_t     sem_size  = 0;      /* number of semiprimes stored   */
static idx_t     sem_limit = 0;      /* highest N for which spf built */

static uint32_t *spf_array = NULL;  /* smallest prime factor array   */
static idx_t     spf_limit  = 0;    /* current limit of spf_array    */

/* Forward declaration */
static void recompute_spf(idx_t limit);
static void extend_sem_list(idx_t needed_index);

/* Build smallest prime factor table up to 'limit' (inclusive) */
static void recompute_spf(idx_t limit) {
    if (limit < 2) return;
    if (spf_array) free(spf_array);
    spf_array = (uint32_t*)calloc((size_t)limit + 1, sizeof(uint32_t));
    if (!spf_array) abort();                       /* allocation error */
    for (idx_t i = 2; i <= limit; ++i) {
        if (spf_array[i] == 0) {                   /* i is prime */
            spf_array[i] = (uint32_t)i;
            if ((uint64_t)i * (uint64_t)i <= limit)
                for (idx_t j = i * i; j <= limit; j += i)
                    if (spf_array[j] == 0) spf_array[j] = (uint32_t)i;
        }
    }
    spf_limit = limit;
}

/* Ensure that sem_list contains at least 'needed_index' semiprimes.
   The array is rebuilt from scratch each time it is enlarged.         */
static void extend_sem_list(idx_t needed_index) {
    if (needed_index <= sem_size) return;          /* already long enough */

    idx_t target_limit = (sem_limit == 0) ? 20 : sem_limit * 2;
    while (true) {
        /* enlarge until we have enough entries                 */
        while (target_limit <= sem_limit) target_limit *= 2;

        /* build spf for the enlarged limit                     */
        recompute_spf(target_limit);

        /* first pass: count the semiprimes                      */
        idx_t count = 0;
        for (idx_t n = 2; n <= target_limit; ++n) {
            idx_t m = n;
            int  fcnt = 0;
            while (m > 1 && fcnt <= 2) {
                ++fcnt;
                m /= spf_array[m];
            }
            if (fcnt == 2) ++count;
        }

        /* second pass: store them                                */
        seq_t *new_list = (seq_t*)malloc((size_t)count * sizeof(seq_t));
        if (!new_list) abort();
        idx_t pos = 0;
        for (idx_t n = 2; n <= target_limit; ++n) {
            idx_t m = n;
            int  fcnt = 0;
            while (m > 1 && fcnt <= 2) {
                ++fcnt;
                m /= spf_array[m];
            }
            if (fcnt == 2) new_list[pos++] = (seq_t)n;
        }

        /* replace old list                                          */
        free(sem_list);
        sem_list  = new_list;
        sem_size  = count;
        sem_limit = target_limit;

        /* if still not enough, double and repeat                  */
        if (sem_size >= needed_index) break;
        target_limit *= 2;   /* will recompute in next loop */
    }
}

/* ------------------------------------------------------------------ */
/*  a(n) storage and searching                                       */
/* ------------------------------------------------------------------ */
static seq_t *a_list   = NULL;   /* cache of computed a(n) values    */
static idx_t  a_size   = 0;      /* number of cached terms           */
static idx_t  a_cap    = 0;      /* capacity of a_list               */

static void ensure_a_cap(idx_t cap) {
    if (a_cap >= cap) return;
    if (a_cap == 0) a_cap = 16;
    while (a_cap < cap) a_cap <<= 1;
    seq_t *tmp = (seq_t*)realloc(a_list, a_cap * sizeof(seq_t));
    if (!tmp) abort();
    a_list = tmp;
}

/* ------------------------------------------------------------------ */
/*  Public function                                                   */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n) {
    if (n == 0) return 0;  /* undefined for n==0, return 0 */

    /* If already computed, just return. */
    if (n <= a_size) return a_list[n-1];

    /* Incremental search for new terms.                       */
    static idx_t candidate      = 1;   /* next integer to test   */
    static idx_t found_count    = 0;   /* how many terms found so far */

    while (found_count < n) {
        ++candidate;                                /* examine candidate   */
        extend_sem_list(candidate);                 /* need semiprime(cand) */
        seq_t sem_val = sem_list[candidate-1];      /* semiprime(candidate) */
        seq_t sum_cand = digit_sum((seq_t)candidate);
        seq_t sum_sem  = digit_sum(sem_val);
        if (sum_cand == sum_sem) {
            ++found_count;
            ensure_a_cap(found_count);
            a_list[found_count-1] = (seq_t)candidate;
        }
    }

    return a_list[n-1];
}
