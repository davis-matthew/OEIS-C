/*
 * A224907  Numbers n such that the sum of reciprocals of even divisors of n >
 *          1.
 * https://oeis.org/A224907
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*A005101(n) – twice the n‑th abundant number
 * Range:     u8 n <= 24, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is defined as a(n)=2*A005101(n), where A005101
   is the sequence of abundant numbers (sum of proper divisors > n).  */

#include <stdlib.h>
#include <stddef.h>

/* Abuse of opaque types – seq_t and idx_t are defined in the build
   system.  The code below treats them as unsigned integer types, so
   all arithmetic operators are valid.  */
static idx_t  sieve_limit   = 0;      /* progress limit for the sigma array */
static idx_t  last_checked  = 0;      /* largest index already scanned for abundant numbers */

static seq_t *sigma  = NULL;          /* sum of divisors array */
static size_t sigma_cap = 0;          /* allocated size of sigma */

static seq_t *abund       = NULL;     /* list of abundant numbers */
static size_t abund_sz    = 0;        /* number of entries stored */
static size_t abund_cap   = 0;        /* allocated capacity of abund */

static void rebuild_sigma(idx_t limit)
{
    free(sigma);
    sigma = (seq_t *)calloc((size_t)limit + 1, sizeof(seq_t));
    if (!sigma) abort();
    for (idx_t i = 1; i <= limit; ++i) {
        seq_t val = (seq_t)i;
        for (idx_t j = i; j <= limit; j += i) {
            sigma[j] += val;
        }
    }
    sigma_cap = limit;
}

static void ensure_n(idx_t n)
{
    if (abund_sz >= n) return;

    /* estimate an upper bound for the sieve; double each time */
    idx_t limit = sieve_limit > 0 ? sieve_limit * 2 : 1000;

    while (abund_sz < n) {
        if (limit <= sieve_limit) limit += 1000;
        sieve_limit = limit;
        rebuild_sigma(limit);

        /* scan only the newly discovered range */
        for (idx_t i = last_checked + 1; i <= limit; ++i) {
            if (sigma[i] > (seq_t)2 * (seq_t)i) {
                if (abund_sz == abund_cap) {
                    size_t new_cap = abund_cap == 0 ? 32 : abund_cap * 2;
                    seq_t *tmp = (seq_t *)realloc(abund, new_cap * sizeof(seq_t));
                    if (!tmp) abort();
                    abund = tmp;
                    abund_cap = new_cap;
                }
                abund[abund_sz++] = (seq_t)i;
            }
        }
        last_checked = limit;
        limit *= 2;   /* next time, search further */
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* out of range according to the spec */
    ensure_n(n);
    return abund[n - 1] * ((seq_t)2);
}
