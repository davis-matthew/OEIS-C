/*
 * A020486  Average of squares of divisors is an integer: numbers k such that
 *          sigma_0(k) divides sigma_2(k).
 * https://oeis.org/A020486
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 541, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow, build_spf, seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS A020486:
   numbers k such that sigma_0(k) divides sigma_2(k). */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/build_spf.h"
#include "../utils/big_pow.h"
#include "../utils/seq_alloc_new_array.h"

static seq_t *darr      = NULL;   /* divisor count tau(k) */
static seq_t *sigma2arr = NULL;   /* sum of squares of divisors sigma2(k) */
static idx_t  cur_limit = 0;      /* maximum index computed so far */

static seq_t *seq      = NULL;    /* sequence entries (the k's) */
static size_t seq_len   = 0;      /* number of terms found */
static size_t seq_cap   = 0;      /* allocated capacity of seq */

/* Ensure data up to index N is available --------------------------------- */
static void ensure_limit(idx_t N)
{
    if (N <= cur_limit) return;

    /* Allocate new arrays for tau and sigma2 up to N */
    seq_t *new_tau = seq_alloc_new_array(N + 1);
    seq_t *new_sigma = seq_alloc_new_array(N + 1);

    /* Copy existing data */
    if (cur_limit > 0) {
        for (idx_t i = 0; i <= cur_limit; ++i) {
            new_tau[i] = darr[i];
            new_sigma[i] = sigma2arr[i];
        }
    }

    /* Initialize entries added due to expansion */
    for (idx_t i = cur_limit + 1; i <= N; ++i) {
        new_tau[i] = 0;
        new_sigma[i] = 0;
    }

    /* Point to the newly allocated tables */
    darr = new_tau;
    sigma2arr = new_sigma;

    /* Initialise data for k = 1 if this is the first call */
    if (cur_limit == 0) {
        darr[0] = 0;
        sigma2arr[0] = 0;
        darr[1] = (seq_t)1;
        sigma2arr[1] = (seq_t)1;

        /* Initialise the sequence array with the first term 1 */
        seq_cap = 16;
        seq = seq_alloc_new_array(seq_cap);
        seq[0] = (seq_t)1;
        seq_len = 1;
        cur_limit = 1;
    }

    /* Build smallest prime factor table up to N */
    uint64_t *spf = NULL;
    build_spf(N, &spf);

    /* Compute tau and sigma2 for all new numbers */
    for (idx_t i = cur_limit + 1; i <= N; ++i) {
        uint64_t p = spf[i];
        idx_t exp = 0;
        uint64_t t = i;
        while (t % p == 0) { t /= p; ++exp; }
        uint64_t m = t;   /* remaining part after removing p^exp */

        /* divisor count */
        darr[i] = darr[m] * (seq_t)(exp + 1);

        /* sigma_2 for p^exp */
        seq_t denom = (seq_t)p * (seq_t)p - (seq_t)1;
        seq_t numer = big_pow((seq_t)p, 2 * (exp + 1)) - (seq_t)1;
        seq_t sigma_part = numer / denom;

        sigma2arr[i] = sigma2arr[m] * sigma_part;

        /* Check the divisibility condition */
        if (sigma2arr[i] % darr[i] == 0) {
            /* Grow the sequence array if necessary */
            if (seq_len == seq_cap) {
                size_t new_cap = seq_cap ? seq_cap * 2 : 16;
                seq_t *new_seq = seq_alloc_new_array(new_cap);
                for (size_t j = 0; j < seq_len; ++j) new_seq[j] = seq[j];
                seq = new_seq;
                seq_cap = new_cap;
            }
            seq[seq_len++] = (seq_t)i;
        }
    }

    cur_limit = N;
    free(spf);
}

/* Public interface -------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for index 0 */

    /* Grow the tables until we have at least n terms */
    while (seq_len < n) {
        idx_t new_limit = cur_limit ? cur_limit << 1 : 64;
        if (new_limit < 64) new_limit = 64;
        ensure_limit(new_limit);
    }
    return seq[n - 1];
}
