/*
 * A131788  a(n) = a(n-1) + (sum of the terms, from among the first (n-1)
 *          terms of the sequence, which are coprime to the n-th Fibonacci
 *          number).
 * https://oeis.org/A131788
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 28, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   fib, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A131788: a(1) = 1; for n > 1,
   a(n) = a(n-1) + Σ_{k=1}^{n-1} a(k)   (where gcd(a(k), F(n)) = 1)   */

#include <stddef.h>   /* NULL, size_t */
#include <stdlib.h>   /* realloc */
#include "../utils/fib.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    static seq_t *A   = NULL;   /* array of computed a(k), 0‑indexed (A[0] = a(1)) */
    static size_t cap = 0;      /* allocated capacity (in elements) */
    static idx_t up_to = 0;     /* largest n for which A[0..up_to-1] are valid */

    if (n == 0) return 0;       /* undefined, but harmless */

    if (n > up_to) {
        /* grow the storage if needed */
        size_t needed = (size_t)n;
        if (needed > cap) {
            size_t newcap = needed > 2 * cap ? needed : 2 * cap;
            seq_t *tmp = (seq_t *)realloc(A, newcap * sizeof(seq_t));
            A   = tmp;
            cap = newcap;
        }

        /* compute all missing values from up_to+1 to n inclusive */
        for (idx_t i = up_to + 1; i <= n; ++i) {
            if (i == 1) {
                A[0] = 1;                     /* a(1) = 1 */
            } else {
                seq_t sum = A[(size_t)(i - 2)];/* start with a(i‑1) */
                seq_t Fn   = fib(i);          /* F(i) */
                for (idx_t k = 1; k < i; ++k) {
                    if (gcd(A[(size_t)(k - 1)], Fn) == 1)
                        sum += A[(size_t)(k - 1)];
                }
                A[(size_t)(i - 1)] = sum;     /* store a(i) */
            }
        }
        up_to = n;
    }
    return A[(size_t)(n - 1)];
}
