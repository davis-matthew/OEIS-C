/*
 * A206741  G.f.: 1/(1 - x/(1 - x/(1 - x^2/(1 - x^3/(1 - x^5/(1 - x^8/(1 -...-
 *          x^Fibonacci(n)/(1 -...)))))))), a continued fraction.
 * https://oeis.org/A206741
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in 1/(1 - x/(1 - x/(1 - x^2/(1 -
 *            x^3/(1 - x^5/(1 - x^8/(1 - ...))))))
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 28, u64 n <= 55, u128 n <= 109,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Compute the n-th term of OEIS A206741.  */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* The build supplies idx_t and seq_t types.  */
seq_t a(idx_t n)
{
    /* Build Fibonacci exponents up to n.  */
    idx_t fib[64];          /* enough for n <= 10¹⁰⁰⁶ (next Fib ≈ 10946) */
    int fibcnt = 0;
    if (n >= 1) {
        fib[fibcnt++] = 1;
        fib[fibcnt++] = 1;
    }
    idx_t f1 = 1, f2 = 1;
    while (1) {
        idx_t f3 = f1 + f2;
        if (f3 > n) break;
        fib[fibcnt++] = f3;
        f1 = f2;
        f2 = f3;
    }

    /* Allocate coefficient arrays.  */
    size_t sz = (size_t)n + 1;
    seq_t *cur  = (seq_t *)calloc(sz, sizeof(seq_t));
    seq_t *next = (seq_t *)calloc(sz, sizeof(seq_t));
    if (!cur || !next) return 0;   /* out of memory */

    cur[0] = 1;  /* base case */

    /* Process exponents from largest to smallest.  */
    for (int ei = fibcnt - 1; ei >= 0; --ei) {
        idx_t e = fib[ei];
        next[0] = 1;   /* constant term stays 1 */

        /* For degrees 1 .. n.  */
        for (size_t k = 1; k <= n; ++k) {
            if (k < e) {
                next[k] = 0;
                continue;
            }
            seq_t sum = 0;
            size_t limit = (size_t)(k - e);
            for (size_t i = 0; i <= limit; ++i) {
                sum += next[i] * cur[limit - i];
            }
            next[k] = sum;
        }

        /* Swap cur and next for next iteration.  */
        seq_t *tmp = cur;
        cur = next;
        next = tmp;
    }

    seq_t result = cur[n];

    free(cur);
    free(next);
    return result;
}
