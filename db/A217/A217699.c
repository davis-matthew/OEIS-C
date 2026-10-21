/*
 * A217699  G.f. A(x) satisfies: 1 - x*A(x) + x^2*A(x)^2 = Sum_{n>=0}
 *          (-x)^(n^2).
 * https://oeis.org/A217699
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   1 - x*A(x) + x^2*A(x)^2 = Σ_{n≥0} (-x)^(n^2) * Recurrence: *
 *            a(0) = 1 * For n ≥ 1, * a[n] = Σ_{i=0}^{n-1} a[i] * a[n-1-i] -
 *            s, * where s = 0 unless n+1 is a perfect square k^2; * if n+1 =
 *            k^2 then s = (k%2==0 ? +1 : -1). * This implements the sequence
 *            A217699
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 19, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Integer square root of an idx_t.  
   If a build‑time helper is available, this will be overridden. */
static inline idx_t isqrt_u64(idx_t n) {
    idx_t lo = 0, hi = ((idx_t)1 << ((sizeof(idx_t)*8 + 1) / 2));
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if (mid && mid > n / mid) hi = mid - 1;
        else lo = mid;
    }
    return lo;
}

seq_t a(idx_t n) {
    static seq_t *buf = NULL;
    static idx_t bufsize = 0;

    /* initialise for n = 0 */
    if (bufsize == 0) {
        bufsize = 1;
        buf = (seq_t *)malloc(sizeof(seq_t));
        if (buf == NULL) exit(EXIT_FAILURE);
        buf[0] = (seq_t)1;
    }

    /* expand cache if necessary */
    if (n + 1 > bufsize) {
        size_t newsize = (size_t)(n + 1);
        seq_t *newbuf = (seq_t *)realloc(buf, newsize * sizeof(seq_t));
        if (newbuf == NULL) exit(EXIT_FAILURE);
        buf = newbuf;

        /* compute all new terms */
        for (idx_t k = bufsize; k <= n; ++k) {
            /* convolution Σ_{i=0}^{k-1} a[i] * a[k-1-i] */
            seq_t sum = (seq_t)0;
            for (idx_t i = 0; i < k; ++i) {
                sum += buf[i] * buf[k - 1 - i];
            }

            /* correction term s */
            idx_t m = k + 1; /* index of the right‑hand side term */
            idx_t r = isqrt_u64(m);
            if (r * r == m) { /* m is a perfect square */
                if ((r & 1) == 0) { /* even r -> s = +1 */
                    sum -= (seq_t)1;
                } else {              /* odd r  -> s = -1 */
                    sum += (seq_t)1;
                }
            }
            buf[k] = sum;
        }
        bufsize = newsize;
    }

    return buf[(size_t)n];
}
