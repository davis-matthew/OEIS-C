/*
 * A096795  Numerator of sum of reciprocals of first n prime powers;
 *          denominator=A051451(n).
 * https://oeis.org/A096795
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/gcd.h"

/* Numerator of the sum of reciprocals of the first n prime powers;
   a(n) = sum_{k=1..n} L / d_k ,  
   where d_k is the k‑th prime power (1,2,3,4,5,7,8,9,11,13,16,17,19,23,25,27,29,31,32,37,41, …)
   and L = lcm(d_1,…,d_n). */

/* Comparator used by qsort for seq_t values. */
static int seq_compare(const void *a, const void *b)
{
    const seq_t *x = (const seq_t *)a;
    const seq_t *y = (const seq_t *)b;
    if (*x < *y) return -1;
    if (*y < *x) return 1;
    return 0;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    /* Estimate an upper bound for the n‑th prime power.
       We use a generous over‑estimate so that the sieve always covers enough numbers. */
    idx_t limit;
    if (n < 6) {
        limit = 20;
    } else {
        double dn = (double)n;
        double est = dn * (log(dn) + log(log(dn))) * 1.5;
        if (est < 20.0) est = 20.0;
        limit = (idx_t)est + 10;
    }

    seq_t *powers = NULL;
    size_t powers_cnt;

    for (;;) {
        /* --- 1.  Sieve to obtain the primes up to limit -------------- */
        bool *is_prime = (bool *)calloc((size_t)(limit + 1), sizeof(bool));
        if (!is_prime) return (seq_t)0;
        for (size_t i = 0; i <= limit; ++i) is_prime[i] = true;
        is_prime[0] = is_prime[1] = false;
        for (idx_t p = 2; (size_t)p * (size_t)p <= limit; ++p) {
            if (is_prime[p]) {
                for (size_t j = (size_t)p * (size_t)p; j <= limit; j += (size_t)p)
                    is_prime[j] = false;
            }
        }

        /* --- 2.  Gather all prime powers up to limit --------------- */
        powers_cnt = 0;
        powers = (seq_t *)malloc((size_t)(limit + 1) * sizeof(seq_t));
        if (!powers) { free(is_prime); return (seq_t)0; }

        powers[powers_cnt++] = (seq_t)1;          /* 1 is the 0‑th prime power */

        for (size_t i = 2; i <= limit; ++i) {
            if (!is_prime[i]) continue;
            seq_t p = (seq_t)i;
            seq_t power = p;
            while (true) {
                powers[powers_cnt++] = power;
                /* Stop if the next power would exceed the limit or overflow */
                if (power > ((seq_t)limit / p))
                    break;
                power *= p;
            }
        }

        free(is_prime);

        /* --- 3.  Sort the powers and test if we have enough -------- */
        qsort(powers, powers_cnt, sizeof(seq_t), seq_compare);

        if (powers_cnt >= n) break;          /* we have at least n prime powers   */
        free(powers);
        limit *= 2;                           /* enlarge and try again            */
    }

    /* --- 4.  Compute the numerator of the sum ------------------- */
    seq_t L = (seq_t)1;  /* current lcm of the powers processed so far */
    seq_t N = (seq_t)1;  /* current numerator (sum = N / L)              */

    for (idx_t i = 2; i <= n; ++i) {          /* powers[0] is 1 */
        seq_t d = powers[i - 1];
        seq_t g  = gcd(L, d);
        seq_t d1 = d / g;
        seq_t L1 = L / g;
        N = N * d1 + L1;
        L = L * d1;
    }

    free(powers);
    return N;                                 /* a(n) */
}
