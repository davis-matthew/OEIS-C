/*
 * A205973  a(n) = Fibonacci(n)*A109041(n) for n>=1, with a(0)=1, where
 *          A109041 lists the coefficients in eta(q)^9/eta(q^3)^3.
 * https://oeis.org/A205973
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Fibonacci(n) * A109041(n) (n ≥ 1), a(0) = 1 * *
 *            A109041(n) are the coefficients of η(q)^9 / η(q^3)^3. * Let c_m
 *            = 9 for m not divisible by 3, otherwise 6. * For the infinite
 *            product Π (1 - q^m)^{c_m} the coefficients, * denoted B(n),
 *            satisfy the recurrence: * * n·B(n) = - Σ_{k=1}^n ( Σ_{d|...
 * Range:     i8 n <= 3, i16 n <= 9, i32 n <= 28, i64 n <= 30, i128 n <= 30,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/fib.h"

/* static storage for the coefficients B(n)  (type __int128) */
static __int128 *coeff_128 = NULL;
static idx_t coeff_cap = 0;   /* highest n already computed */

seq_t a(idx_t n)
{
    /* a(0) is defined explicitly */
    if (n == (idx_t)0) {
        return (seq_t)1;
    }

    /* extend the coefficient table if necessary */
    if (n > coeff_cap) {
        idx_t new_cap = n;
        /* reallocate coefficient array to hold 0 .. new_cap */
        coeff_128 = ( __int128 * )realloc(coeff_128, sizeof(__int128) * (new_cap + 1));
        if (!coeff_128) abort();

        /* temporary array for S(k) = Σ_{d|k} d·c_d  (k = 0..new_cap) */
        long long *S = (long long *)malloc(sizeof(long long) * (new_cap + 1));
        if (!S) abort();

        /* initialise S to zero */
        for (idx_t i = 0; i <= new_cap; ++i) S[i] = 0;

        /* compute S(k) by the divisor sieve */
        for (idx_t d = 1; d <= new_cap; ++d) {
            long long c = (d % 3 == 0) ? 6 : 9;   /* exponent of (1 - q^d) */
            long long add = c * (long long)d;
            for (idx_t m = d; m <= new_cap; m += d) {
                S[m] += add;
            }
        }

        /* recurrence to compute B(n) = A109041(n) */
        coeff_128[0] = ( __int128 )1;             /* B(0) = 1 */
        for (idx_t i = 1; i <= new_cap; ++i) {
            __int128 sum = 0;
            for (idx_t k = 1; k <= i; ++k) {
                sum += (__int128)S[k] * coeff_128[i - k];
            }
            /* n*B(n) = -sum  →  B(n) = -sum / n */
            coeff_128[i] = -sum / ( __int128 )i;
        }

        free(S);
        coeff_cap = new_cap;
    }

    /* Fibonacci(n) times the coefficient B(n) */
    seq_t fib_n = fib(n);
    seq_t coeff_seq = (seq_t)coeff_128[n];   /* cast to seq_t (may truncate) */
    return fib_n * coeff_seq;
}
