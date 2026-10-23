/*
 * A190118  a(n) = Sum_{k=1..n} k/gcd(k,k'), where n' is arithmetic derivative
 *          of n.
 * https://oeis.org/A190118
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} k/gcd(k,k') * where k' is the arithmetic
 *            derivative of k. * Uses a simple SPF sieve for factorisation
 * Range:     u8 n <= 28, u16 n <= 438, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n) {
    /* Static state for Sieve of Eratosthenes */
    static idx_t max_n = 0;
    static uint64_t *spf = NULL;

    /* grow the spf array if needed */
    if (n > max_n) {
        uint64_t *new_spf = (uint64_t*)realloc(spf, (size_t)(n + 1) * sizeof(uint64_t));
        if (!new_spf) return 0; /* allocation failure, fall back */
        spf = new_spf;
        for (size_t i = max_n + 1; i <= n; ++i) spf[i] = i;
        for (size_t i = 2; i * i <= n; ++i) {
            if (spf[i] == i) {            /* i is prime */
                for (size_t j = i * i; j <= n; j += i)
                    if (spf[j] == j) spf[j] = i;
            }
        }
        max_n = n;
    }

    seq_t total = 0;

    for (idx_t k = 1; k <= n; ++k) {
        seq_t kk = (seq_t)k;
        seq_t derivative = 0;

        if (k != 1) {                      /* k' = 0 for k = 1 */
            idx_t tmp = k;
            while (tmp > 1) {
                uint64_t p = spf[tmp];
                int exp = 0;
                while (tmp % p == 0) {
                    tmp /= p;
                    exp++;
                }
                /* term = exp * (k / p)  */
                seq_t term = (seq_t)exp * (kk / p);
                derivative += term;
            }
        }

        seq_t g = gcd(kk, derivative);     /* gcd(1,0)=1 by definition */
        total += kk / g;
    }

    return total;
}
