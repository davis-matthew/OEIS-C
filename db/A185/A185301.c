/*
 * A185301  G.f.: exp( Sum_{n>=1} Sum_{k>=1} sigma(n)^k * x^(n*k) / n ).
 * https://oeis.org/A185301
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 28, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A185301.
   Formula: let σ(k) = sum of divisors of k.
   Define c(k) = sum_{d|k} (k/d) * σ(d)^(k/d).
   Then a(0)=1 and for n>0

       a(n) = (1/n) * Σ_{k=1}^{n} c(k) * a(n-k).
*/
#include <stdlib.h>
#include <stdint.h>
#include "../utils/big_pow.h"   /* seq_t big_pow(seq_t base, seq_t exp) */

seq_t a(idx_t n) {
    /* Handle the offset directly. */
    if (n == 0) return (seq_t)1;

    static seq_t *A    = NULL;  /* a values   */
    static seq_t *sigma = NULL; /* σ values   */
    static seq_t *c   = NULL;   /* c values   */
    static idx_t max_n = 0;     /* largest index computed */

    if (n > max_n) {
        /* Grow the storage to accommodate up to n. */
        idx_t new_n = n;
        size_t sz = (size_t)new_n + 1;   /* indices 0 .. new_n inclusive */

        A     = (seq_t *)realloc(A,     sz * sizeof(seq_t));
        sigma = (seq_t *)realloc(sigma, sz * sizeof(seq_t));
        c     = (seq_t *)realloc(c,     sz * sizeof(seq_t));

        /* Zero the new arrays (all entries will be recomputed). */
        for (size_t i = 0; i < sz; ++i) {
            A[i] = (seq_t)0;
            sigma[i] = (seq_t)0;
            c[i] = (seq_t)0;
        }

        /* Compute σ(k) for all k ≤ new_n. */
        for (idx_t i = 1; i <= new_n; ++i) {
            seq_t add = (seq_t)i;
            for (idx_t j = i; j <= new_n; j += i) {
                sigma[j] += add;
            }
        }

        /* Compute c(k) for all k ≤ new_n. */
        for (idx_t k = 1; k <= new_n; ++k) {
            seq_t sum = (seq_t)0;
            for (idx_t d = 1; d <= k / d; ++d) {
                if (k % d == 0) {
                    idx_t e = k / d;                       /* e = k/d  */
                    seq_t pow1 = big_pow(sigma[d], (seq_t)e);/* σ(d)^e  */
                    sum += (seq_t)e * pow1;                /* e * σ(d)^e */
                    if (e != d) {                         /* another divisor */
                        seq_t pow2 = big_pow(sigma[e], (seq_t)d);/* σ(e)^d */
                        sum += (seq_t)d * pow2;            /* d * σ(e)^d */
                    }
                }
            }
            c[k] = sum;
        }

        /* Compute a(0..new_n) using the recurrence. */
        A[0] = (seq_t)1;
        for (idx_t m = 1; m <= new_n; ++m) {
            seq_t acc = (seq_t)0;
            for (idx_t k = 1; k <= m; ++k) {
                acc += c[k] * A[m - k];
            }
            A[m] = acc / (seq_t)m;   /* division is exact */
        }

        max_n = new_n;
    }

    return A[n];
}
