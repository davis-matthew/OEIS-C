/*
 * A023885  Nonprimes whose average of proper divisors is an integer.
 * https://oeis.org/A023885
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 4395, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_spf, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A023885 (OEIS)
   a(n) = the n‑th composite integer for which the average of its proper
   divisors is an integer.
   Equivalent condition: (σ(n) – n)  mod  (τ(n) – 1)  = 0
   where σ is the sum of all divisors and τ is the number of divisors.
   Implementation uses a dynamic SPF sieve and searches forward until
   the n‑th qualifying number is found.  This runs in O(b) time where
   b is the value of the n‑th term (≈ O(n log n)). */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime.h"        /* is_prime(idx_t) */
#include "../utils/build_spf.h"      /* build_spf(idx_t, idx_t ** ) */

/* SPF table (smallest prime factor) */
static idx_t *spf_tbl = NULL;   /* array indexed by integer value */
static idx_t spf_limit = 0;     /* size of the table */

/* Make sure the SPF table covers all numbers up to `need`. */
static void ensure_spf(idx_t need)
{
    if (spf_limit >= need) return;          /* already sufficient */

    /* Reallocate: free old table if any */
    if (spf_tbl) free(spf_tbl);
    spf_tbl = NULL;
    build_spf(need, &spf_tbl);              /* allocates a new table */
    spf_limit = need;                      /* update size */
}

/* Compute σ(n) and τ(n) for a number ≤ spf_limit.  As the number is
   guaranteed to fit in idx_t, we can perform the calculations using
   seq_t arithmetic without overflow. */
static void sigma_tau(idx_t n, seq_t *sigma, seq_t *tau)
{
    idx_t  temp = n;
    *sigma = 1;
    *tau   = 1;

    while (temp > 1) {
        idx_t p = spf_tbl[temp];
        int e = 0;
        while ((temp % p) == 0) {
            temp /= p;
            ++e;
        }

        /* σ(p^e) = 1 + p + p^2 + ... + p^e */
        seq_t term = 1;
        seq_t powp = 1;
        for (int i = 0; i < e; ++i) {
            powp = powp * (seq_t)p;
            term = term + powp;
        }
        *sigma = *sigma * term;
        *tau   = *tau * (seq_t)(e + 1);
    }
}

/* Return the n‑th term of the sequence (a(1) = 6). */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* undefined below offset */

    /* Start searching from the first composite that can qualify. */
    idx_t number = 4;          /* 4 is the smallest composite */
    idx_t found  = 0;

    /* Ensure an initial SPF table large enough for the early numbers. */
    ensure_spf(1000);

    for (;;) {
        /* Extend the SPF table if the current number is beyond it. */
        if (number > spf_limit) {
            idx_t new_limit = spf_limit ? spf_limit : 1000;
            while (new_limit < number) new_limit <<= 1;
            ensure_spf(new_limit);
        }

        if (!is_prime(number)) {        /* candidate must be non‑prime */
            seq_t sig, tau;
            sigma_tau(number, &sig, &tau);
            seq_t proper_sum = sig - (seq_t)number;
            seq_t proper_cnt = tau - 1;   /* exclude the number itself */

            if (proper_cnt != 0 && (proper_sum % proper_cnt) == 0) {
                ++found;
                if (found == n) return (seq_t)number;
            }
        }
        ++number;
    }
}
