/*
 * A101405  a(n) = n^(pi(n-1)).
 * https://oeis.org/A101405
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^π(n-1) where π(x) is the number of primes ≤ x
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 19, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/pow_seq.h"   /* provides seq_t pow_seq(seq_t base, idx_t exp) */

static idx_t *pi_table = NULL;       /* cumulative prime counts */
static bool *prime_mark = NULL;      /* sieve marks */
static idx_t  max_cached = 0;        /* maximum index cached in tables */

static void ensure_sieve_up_to(idx_t up)
{
    if (up <= max_cached) return;

    /* Resize arrays */
    if (pi_table == NULL) {
        pi_table   = (idx_t*)malloc((up + 1) * sizeof(idx_t));
        prime_mark = (bool*)malloc((up + 1) * sizeof(bool));
    } else {
        pi_table   = (idx_t*)realloc(pi_table, (up + 1) * sizeof(idx_t));
        prime_mark = (bool*)realloc(prime_mark, (up + 1) * sizeof(bool));
    }

    /* Initialize new portion of sieve */
    for (idx_t i = max_cached + 1; i <= up; ++i) prime_mark[i] = true;
    if (up >= 0) prime_mark[0] = false;
    if (up >= 1) prime_mark[1] = false;

    /* Sieve only new range if we already had some */
    for (idx_t i = 2; i * i <= up; ++i) {
        if (prime_mark[i]) {
            for (idx_t j = (i * i > max_cached + 1 ? i * i : ((max_cached + 1 + i - 1) / i) * i);
                 j <= up; j += i) {
                prime_mark[j] = false;
            }
        }
    }

    /* Build prefix prime counts */
    if (max_cached == 0) pi_table[0] = 0;
    else pi_table[max_cached] = pi_table[max_cached - 1] + (prime_mark[max_cached] ? 1 : 0);
    for (idx_t i = max_cached + 1; i <= up; ++i) {
        pi_table[i] = pi_table[i - 1] + (prime_mark[i] ? 1 : 0);
    }

    max_cached = up;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* out of OEIS range, but safe */
    if (n == 1) return (seq_t)1;   /* 1^π(0) = 1 */

    /* Ensure we have prime counts up to n-1 */
    ensure_sieve_up_to(n - 1);

    idx_t pi_val = pi_table[n - 1];
    return pow_seq((seq_t)n, pi_val);
}
