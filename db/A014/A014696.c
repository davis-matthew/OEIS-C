/*
 * A014696  Poincaré series [or Poincare series] (or Molien series) for mod 2
 *          cohomology of universal W-group W(3).
 * https://oeis.org/A014696
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = coefficients from the generating function
 *            (1+x+5x^2+x^3+x^4)/((1-x)^2*(1-x^2)^4) Recurrence (order 10):
 *            a(n) = 2*a(n‑1) + 3*a(n‑2) +12*a(n‑5) + 3*a(n‑8) + 2*a(n‑9) –
 *            8*a(n‑3) – 2*a(n‑4) – 2*a(n‑6) – 8*a(n‑7) – a(n‑10) for n ≥ 10
 *            with a(0)…a(9) given below
 * Range:     u8 n <= 5, u16 n <= 23, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init[10] = {
    (seq_t)1,    /* a(0) */
    (seq_t)3,    /* a(1) */
    (seq_t)14,   /* a(2) */
    (seq_t)30,   /* a(3) */
    (seq_t)77,   /* a(4) */
    (seq_t)138,  /* a(5) */
    (seq_t)273,  /* a(6) */
    (seq_t)438,  /* a(7) */
    (seq_t)748,  /* a(8) */
    (seq_t)1113  /* a(9) */
};

seq_t a(idx_t n)
{
    /* Return the first ten values directly. */
    if (n < 10)
        return init[n];

    /* Allocate space for terms 0 … n. */
    size_t len = (size_t)(n + 1);
    seq_t *seq = (seq_t *)malloc(sizeof(seq_t) * len);
    if (!seq)
        return (seq_t)0;   /* allocation failure → zero (unlikely in tests). */

    /* Copy the known initial terms. */
    for (size_t i = 0; i < 10; ++i)
        seq[i] = init[i];

    /* Compute subsequent terms using the recurrence. */
    for (idx_t i = 10; i <= n; ++i) {
        seq_t pos = seq[i-1] * (seq_t)2
                  + seq[i-2] * (seq_t)3
                  + seq[i-5] * (seq_t)12
                  + seq[i-8] * (seq_t)3
                  + seq[i-9] * (seq_t)2;

        seq_t neg = seq[i-3] * (seq_t)8
                  + seq[i-4] * (seq_t)2
                  + seq[i-6] * (seq_t)2
                  + seq[i-7] * (seq_t)8
                  + seq[i-10];

        seq[i] = pos - neg;   /* final term, positive for all n. */
    }

    seq_t result = seq[n];
    free(seq);               /* release the temporary array. */
    return result;
}
