/*
 * A049701  Duplicate of A005598.
 * https://oeis.org/A049701
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(0)=0, a(1)=2, for n≥2: a(n)=a(n-1)+Σ_{k=1}^{n} φ(k) (φ is
 *            Euler's totient function)
 * Range:     i8 n <= 9, i16 n <= 19, i32 n <= 19, i64 n <= 19, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 2;

    /* Allocate array for φ(0)…φ(n) as 64‑bit integers (n is at most a few 10⁴). */
    uint64_t *phi = (uint64_t *)malloc((n + 1) * sizeof(uint64_t));
    if (!phi) return 0;          /* fallback on allocation failure */

    for (idx_t i = 0; i <= n; ++i)
        phi[i] = i;

    /* Euler's totient sieve */
    for (idx_t i = 2; i <= n; ++i) {
        if (phi[i] == i) {       /* i is prime */
            for (idx_t j = i; j <= n; j += i)
                phi[j] -= phi[j] / i;
        }
    }

    /* Build the sequence using the recurrence */
    seq_t cum = (seq_t)phi[1];   /* cum = Σ_{k=1}^{1} φ(k) */
    seq_t res = 2;               /* a(1) */

    for (idx_t i = 2; i <= n; ++i) {
        cum += (seq_t)phi[i];    /* cum = Σ_{k=1}^{i} φ(k) */
        res += cum;              /* a(i) = a(i-1) + cum          */
    }

    free(phi);
    return res;
}
