/*
 * A128647  a(n) = numerator(Sum_{k=1..n} (-1)^(k+1)/(prime(k)-1)).
 * https://oeis.org/A128647
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = numerator( Σ_{k=1..n} (-1)^(k+1)/(prime(k)-1) )
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 22, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   gcd, sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/sieve_first_n_primes.h"

seq_t a(idx_t n) {
    /* Keep a cache of primes so that repeated calls are fast. */
    static uint32_t *primes = NULL;      /* array of first primes */
    static size_t primes_cnt = 0;        /* number of primes in the cache */

    if (n == 0) return 0; /* undefined offset, but return 0 for safety */

    /* Ensure we have at least n primes cached. */
    if (n > primes_cnt) {
        /* free previous allocation if any */
        free(primes);
        primes = NULL;
        sieve_first_n_primes(n, &primes, &primes_cnt);
    }

    seq_t sum_num = 0;       /* numerator of the current sum (non‑negative) */
    seq_t sum_den = 1;       /* denominator of the current sum */
    int sum_sign = 1;        /* sign of the current sum (+1 or -1) */

    for (idx_t k = 1; k <= n; ++k) {
        uint32_t p = primes[k - 1];              /* the k‑th prime */
        seq_t term_den = (seq_t)p - 1;           /* denominator of the term */
        int term_sign = (k & 1) ? 1 : -1;        /* (-1)^(k+1) */

        /* Compute new denominator: lcm(sum_den, term_den) */
        seq_t g = gcd(sum_den, term_den);
        seq_t b_div_g = sum_den / g;
        seq_t d_div_g = term_den / g;
        seq_t new_den = b_div_g * term_den;      /* = sum_den / g * term_den */

        /* Compute numerator parts */
        seq_t part1 = sum_num * d_div_g;         /* sum_num * (term_den / g)   */
        seq_t part2 = b_div_g;                   /* 1 * (sum_den / g)          */

        seq_t new_num;
        int new_sign;

        if (sum_sign == term_sign) {
            /* Same sign, just add magnitudes */
            new_num = part1 + part2;
            new_sign = sum_sign;
        } else {
            /* Opposite signs, subtract smaller magnitude from larger */
            if (part1 >= part2) {
                new_num = part1 - part2;
                new_sign = sum_sign;
            } else {
                new_num = part2 - part1;
                new_sign = term_sign;
            }
        }

        /* Reduce the fraction */
        seq_t g2 = gcd(new_num, new_den);
        sum_num = new_num / g2;
        sum_den = new_den / g2;
        sum_sign = new_sign;
    }

    /* The sequence definition expects the numerator in lowest terms. */
    return sum_num;
}
