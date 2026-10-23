/*
 * A023649  Convolution of composite numbers and (F(2), F(3), F(4), ...).
 * https://oeis.org/A023649
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=1}^{n} C(k) * F(n-k+2) where C(k) is the k‑th
 *            composite number and F(m) is the Fibonacci sequence (F(0)=0,
 *            F(1)=1). The recurrence below follows: a(1)=4, a(2)=14
 *            a(n)=a(n-1)+a(n-2)+C(n-1)+C(n) for n≥3
 * Range:     u8 n <= 6, u16 n <= 17, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* not part of the OEIS offset */
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)14;

    /* Generate the first n composite numbers */
    seq_t *comp = (seq_t *)malloc(sizeof(seq_t) * (size_t)n);
    if (!comp) return 0;             /* allocation failure – unlikely */

    idx_t candidate = 4;             /* first composite */
    size_t cnt = 0;
    while (cnt < (size_t)n) {
        if (!is_prime(candidate)) {
            comp[cnt] = (seq_t)candidate;
            ++cnt;
        }
        ++candidate;
    }

    /* Iterate the recurrence */
    seq_t a_prev2 = 4;                /* a(1) */
    seq_t a_prev1 = 14;               /* a(2) */
    for (idx_t i = 3; i <= n; ++i) {
        seq_t ci_minus1 = comp[i - 2];   /* C(i-1) */
        seq_t ci        = comp[i - 1];   /* C(i)   */
        seq_t a_curr = a_prev1 + a_prev2 + ci_minus1 + ci;
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }

    free(comp);
    return a_prev1;    /* a(n) for n≥2 */
}
