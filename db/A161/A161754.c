/*
 * A161754  a(n) = sum of all nonprimes from nonprime(p) to nonprime(q) where
 *          p is nonprime(n+1) and q is nonprime(n+2).
 * https://oeis.org/A161754
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{i=nonprime(n+1)}^{nonprime(n+2)} nonprime(i)
 * Range:     u8 n <= 47, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_small.h"

static seq_t *nonprimes   = NULL;   /* nonprime(i) stored at nonprimes[i] (1‑based) */
static seq_t *prefix_sums = NULL;   /* prefix_sums[i] = Σ_{k=1}^{i} nonprimes[k] */
static idx_t   nonprime_cnt = 0;    /* number of nonprimes currently stored */
static idx_t   nonprime_cap = 0;    /* allocated capacity (index maximum) */
static uint64_t next_candidate = 0; /* next integer to test for primality */

/* Ensure nonprimes[1..max_idx] and the corresponding prefix sums exist. */
static void ensure_nonprimes_up_to(idx_t max_idx)
{
    while (nonprime_cnt <= max_idx) {
        uint64_t val = next_candidate++;                  /* candidate integer */

        bool is_prime = (val >= 2) && is_prime_small((seq_t)val);
        if (!is_prime) {                                 /* val is a nonprime */

            if (nonprime_cnt + 1 > nonprime_cap) {        /* grow storage */
                idx_t new_cap = nonprime_cap ? nonprime_cap << 1 : 16;
                nonprimes   = (seq_t*)realloc(nonprimes,   (new_cap + 1) * sizeof(seq_t));
                prefix_sums = (seq_t*)realloc(prefix_sums, (new_cap + 1) * sizeof(seq_t));
                if (!nonprimes || !prefix_sums) abort();
                if (nonprime_cap == 0) {                  /* first allocation */
                    prefix_sums[0] = (seq_t)0;
                }
                nonprime_cap = new_cap;
            }

            idx_t new_index = nonprime_cnt + 1;            /* 1‑based index */
            nonprimes[new_index]   = (seq_t)val;
            prefix_sums[new_index] = prefix_sums[new_index - 1] + (seq_t)val;
            nonprime_cnt = new_index;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                         /* undefined for n<1 */

    /* First level: need nonprime(n+1) and nonprime(n+2) as indices. */
    ensure_nonprimes_up_to(n + 2);

    idx_t p = (idx_t)nonprimes[n + 1];
    idx_t q = (idx_t)nonprimes[n + 2];

    /* Second level: need prefix sums up to index q. */
    ensure_nonprimes_up_to(q);

    seq_t sum = prefix_sums[q] - prefix_sums[p - 1];
    return sum;
}
