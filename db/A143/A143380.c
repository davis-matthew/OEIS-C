/*
 * A143380  Expansion of q^(-1/6) * eta(q^2)^5 / (eta(q)^2 * eta(q^4)) in
 *          powers of q.
 * https://oeis.org/A143380
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A143380: coefficient of q^n in q^(-1/6) * eta(q^2)^5/(eta(q)^2*eta(q^4)).
   Implemented using the product form
   ∏_{k≥1}(1-(-x)^k)^2(1+x^{2k})
   which expands to
   1 - 2(-1)^k x^k + 2x^{2k} - 2(-1)^k x^{3k} + x^{4k}   for each k. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *buf = NULL;
    static size_t buf_size = 0;
    size_t N = (size_t)n;

    /* Allocate or enlarge buffer as needed.  */
    if (buf_size < N + 1) {
        free(buf);
        buf = (seq_t *)malloc((N + 1) * sizeof(seq_t));
        if (!buf) return 0;          /* return 0 on allocation failure */
        buf_size = N + 1;
    }

    /* initialise array */
    for (size_t i = 0; i <= N; ++i) buf[i] = 0;
    buf[0] = 1;

    /* product iteration */
    for (size_t k = 1; k <= N; ++k) {
        /* coefficient for k and 3k terms: -2*(-1)^k */
        int coeff = (k & 1 ? 2 : -2);

        /* update in reverse order to keep old values for lower indices */
        for (long long i = (long long)N; i >= 0; --i) {
            seq_t val = buf[i];

            if ((size_t)i >= k)
                val += (seq_t)coeff * buf[i - k];
            if ((size_t)i >= 2 * k)
                val += (seq_t)2 * buf[i - 2 * k];
            if ((size_t)i >= 3 * k)
                val += (seq_t)coeff * buf[i - 3 * k];
            if ((size_t)i >= 4 * k)
                val += buf[i - 4 * k];

            buf[i] = val;
        }
    }

    return buf[N];
}
