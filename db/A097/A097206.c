/*
 * A097206  Numbers of the form 5^p * p^5 for p prime.
 * https://oeis.org/A097206
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5^p * p^5 where p is the nth prime
 * Range:     u8 none, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Helpers:   _prime_limit_for_idx, build_primes_upto, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

/* Utilities for prime generation and exponentiation */
#include "../utils/_prime_limit_for_idx.h"
#include "../utils/build_primes_upto.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* 1‑based offset: a(1) is the first term. */
    if (n == 0) return 0;

    /* Global cache of primes */
    static uint64_t *primes   = NULL;
    static size_t   prime_cnt = 0;
    static idx_t    prime_lim = 0;
    static bool     primes_ok = false;

    /* Ensure we have at least the first n primes cached. */
    if (!primes_ok || n > prime_cnt) {
        /* Upper bound on the nth prime. */
        idx_t lim = _prime_limit_for_idx(n);

        /* Free old array if a larger limit is needed. */
        if (lim > prime_lim) {
            free(primes);
            build_primes_upto(lim, &primes, &prime_cnt);
            prime_lim = lim;
            primes_ok = true;
        }
        /* If limit is not large enough, generate again with a bigger bound
           until we have enough primes.  This is safe and fast for the
           required range. */
        else if (prime_cnt < (size_t)n) {
            do {
                lim *= 2;
                free(primes);
                build_primes_upto(lim, &primes, &prime_cnt);
            } while (prime_cnt < (size_t)n);
            prime_lim = lim;
            primes_ok = true;
        }
    }

    uint64_t p = primes[n - 1];          /* nth prime (1‑based) */

    /* Compute 5^p and p^5 using arbitrary‑precision power. */
    seq_t five_pow = seq_pow((seq_t)5, p);
    seq_t prime_pow = seq_pow((seq_t)p, (idx_t)5);

    return five_pow * prime_pow;
}
