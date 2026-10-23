/*
 * A164853  a(n) = lcm(first n semiprimes).
 * https://oeis.org/A164853
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 8, u32 n <= 15, u64 n <= 32, u128 n <= 57,
 *            big any n
 * Helpers:   build_primes_upto, seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A164853:  a(n) = lcm(first n semiprimes).
   Computation: generate all semiprimes up to a dynamic limit, sort,
   then build the cumulative LCM list.  Uses the helpers
   build_primes_upto, seq_gcd, and standard library functions. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"   /* build_primes_upto(idx_t limit,
                                            uint64_t **primes_out,
                                            size_t *cnt_out)           */
#include "../utils/seq_gcd.h"             /* seq_t seq_gcd(seq_t a, seq_t b) */

/* Compare two seq_t values for qsort. */
static int cmp_seq(const void *p1, const void *p2)
{
    seq_t a = ((const seq_t *)p1)[0];
    seq_t b = ((const seq_t *)p2)[0];
    return (a < b) ? -1 : (a > b) ? 1 : 0;
}

/* Static state variables */
static seq_t *semis = NULL;      /* sorted list of semiprimes */
static size_t semis_len = 0;     /* number of semiprimes stored */

static seq_t *lcmlist = NULL;    /* lcm of first k semiprimes, 1-indexed */
static size_t lcmlist_len = 0;   /* current size of lcmlist */

static idx_t current_limit = 0;  /* current prime upper bound */

/* Ensure that semis and lcmlist contain at least 'n' entries. */
static void ensure_terms(idx_t n)
{
    if (lcmlist_len >= (size_t)n)
        return; /* already sufficient */

    /* Need to grow the list of semiprimes.  Double the limit until enough. */
    idx_t target_limit = (current_limit < 50000) ? 50000 : current_limit;
    while (true) {
        /* Grow limit until we get enough semiprimes. */
        target_limit = (current_limit < target_limit) ? target_limit
                                                       : target_limit * 2;
        /* Collect primes up to target_limit. */
        uint64_t *primes_u = NULL;
        size_t primes_cnt = 0;
        build_primes_upto((idx_t)target_limit,
                          &primes_u, &primes_cnt);

        /* Generate all semiprimes p*q with p <= q and p*q <= target_limit. */
        size_t cap = primes_cnt * 2 + 10;
        seq_t *tmp = (seq_t *)malloc(cap * sizeof(seq_t));
        size_t tmp_len = 0;
        for (size_t i = 0; i < primes_cnt; ++i) {
            uint64_t p = primes_u[i];
            for (size_t j = i; j < primes_cnt; ++j) {
                uint64_t q = primes_u[j];
                uint64_t prod = p * q;
                if (prod > (uint64_t)target_limit)
                    break;
                if (tmp_len == cap) {
                    cap *= 2;
                    tmp = (seq_t *)realloc(tmp, cap * sizeof(seq_t));
                }
                tmp[tmp_len++] = (seq_t)prod;
            }
        }
        free(primes_u);

        /* Sort the semiprimes. */
        qsort(tmp, tmp_len, sizeof(seq_t), cmp_seq);

        /* Replace old list. */
        free(semis);
        semis = tmp;
        semis_len = tmp_len;
        current_limit = target_limit;

        if (semis_len >= (size_t)n)
            break;   /* enough semiprimes */
    }

    /* Recompute lcm list up to semis_len. */
    seq_t *new_lcm = (seq_t *)realloc(lcmlist, semis_len * sizeof(seq_t));
    seq_t cur_lcm = (seq_t)1;
    for (size_t i = 0; i < semis_len; ++i) {
        seq_t g = seq_gcd(cur_lcm, semis[i]);
        cur_lcm = (cur_lcm / g) * semis[i];
        new_lcm[i] = cur_lcm;
    }
    lcmlist = new_lcm;
    lcmlist_len = semis_len;
}

/* Public API: a(n) = lcm(first n semiprimes) */
seq_t a(idx_t n)
{
    if (n == 0)   /* undefined, but return 1 for safety */
        return (seq_t)1;
    ensure_terms(n);
    return lcmlist[(size_t)n - 1];
}
