/*
 * A227982  Numbers n such that sigma(n+1) - sigma(n-1) > sigma(n); where
 *          sigma(n) = A000203(n) = sum of the divisors of n.
 * https://oeis.org/A227982
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 741, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search algorithm for A227982: find numbers n such that
   sigma(n+1) - sigma(n-1) > sigma(n) (sigma = sum of divisors). */

#include <stdlib.h>
#include <stdint.h>

/* ----------  Cumulative divisor‑sum sieve  ---------- */

static seq_t *sigma_cache = NULL;   /* sigma_cache[i] = sigma(i) */
static idx_t sigma_limit = 0;       /* maximum i for which sigma_cache is valid */

static void extend_sigma(idx_t required)
{
    if (required <= sigma_limit) return;

    idx_t new_limit = sigma_limit > 0 ? sigma_limit * 2 : 1;
    while (new_limit < required) new_limit *= 2;

    seq_t *new_arr = (seq_t *)malloc(sizeof(seq_t) * (new_limit + 1));
    if (!new_arr) exit(1);

    new_arr[0] = (seq_t)0;
    for (idx_t i = 1; i <= new_limit; ++i) new_arr[i] = (seq_t)1;

    for (idx_t i = 2; i <= new_limit; ++i) {
        seq_t inc = (seq_t)i;
        for (idx_t j = i; j <= new_limit; j += i) {
            new_arr[j] += inc;
        }
    }

    free(sigma_cache);
    sigma_cache = new_arr;
    sigma_limit = new_limit;
}

/* ----------  Sequence storage  ---------- */

static seq_t *seq_arr   = NULL;   /* found integers that satisfy the inequality */
static size_t seq_cap   = 0;      /* capacity of seq_arr */
static idx_t  seq_count = 0;      /* number of integers stored so far */
static idx_t  checked_to = 2;     /* next integer whose property will be tested */

/* ----------  Main sequence function  ---------- */

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined but harmless */

    while (seq_count < n) {
        idx_t m = checked_to++;
        extend_sigma(m + 1);                   /* need sigma(m+1) */

        seq_t sp = sigma_cache[m - 1];         /* sigma(m-1) */
        seq_t sc = sigma_cache[m];             /* sigma(m)   */
        seq_t sn = sigma_cache[m + 1];         /* sigma(m+1) */

        if (sn > sp + sc) {                    /* inequality holds */
            if (seq_count == seq_cap) {
                size_t new_cap = seq_cap ? seq_cap * 2 : 4;
                seq_t *tmp = (seq_t *)realloc(seq_arr, new_cap * sizeof(seq_t));
                if (!tmp) exit(1);
                seq_arr = tmp;
                seq_cap = new_cap;
            }
            seq_arr[seq_count++] = (seq_t)m;
        }
    }

    return seq_arr[n - 1];
}
