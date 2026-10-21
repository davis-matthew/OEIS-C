/*
 * A211723  Number of -3..3 arrays x(i) of n+1 elements i=1..n+1 with
 *          set{t,u,v in 0,1}((x[i+t]+x[j+u]+x[k+v])*(-1)^(t+u+v)) having two,
 *          four or five distinct values for every i,j,k<=n.
 * https://oeis.org/A211723
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) + 2*a(n-2) - 25*a(n-3) + 16*a(n-4) * + 46*a(n-5)
 *            - 48*a(n-6) - 26*a(n-7) + 36*a(n-8) * + 4*a(n-9) - 8*a(n-10) *
 *            Initial values: * a(1) = 48 * a(2) = 94 * a(3) = 162 * a(4) =
 *            294 * a(5) = 508 * a(6) = 922 * a(7) = 1640 * a(8) = 3032 * a(9)
 *            = 5562 * a(10) = 10484
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 29, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /* cache of previously computed values (1‑based index) */
    static seq_t *buf = NULL;
    static size_t buf_cap = 0;
    static idx_t buf_max = 0;

    /* base values for n = 1 .. 10 */
    static const seq_t base_vals[11] = {
        0ULL,        /* index 0 unused */
        48ULL, 94ULL, 162ULL, 294ULL, 508ULL,
        922ULL, 1640ULL, 3032ULL, 5562ULL, 10484ULL
    };

    if (n <= 10) {
        return base_vals[n];
    }

    /* Allocate or grow buffer if necessary */
    if (buf == NULL) {
        /* allocate at least 11 entries to store base values */
        buf_cap = (size_t)n + 1;
        buf = (seq_t *)malloc(buf_cap * sizeof(seq_t));
        if (!buf) abort();
        for (size_t i = 0; i < buf_cap; ++i) buf[i] = 0;
        /* store base values */
        for (idx_t i = 1; i <= 10; ++i) buf[i] = base_vals[i];
        buf_max = 10;
    } else if ((size_t)n + 1 > buf_cap) {
        size_t new_cap = buf_cap * 2;
        if (new_cap < (size_t)n + 1) new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(buf, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        buf = tmp;
        for (size_t i = buf_cap; i < new_cap; ++i) buf[i] = 0;
        buf_cap = new_cap;
        /* base values are already in place */
    }

    /* Compute values up to n */
    for (idx_t k = buf_max + 1; k <= n; ++k) {
        seq_t pos = 4ULL * buf[k-1] + 2ULL * buf[k-2]
                  + 16ULL * buf[k-4] + 46ULL * buf[k-5]
                  + 36ULL * buf[k-8] + 4ULL * buf[k-9];
        seq_t neg = 25ULL * buf[k-3] + 48ULL * buf[k-6]
                  + 26ULL * buf[k-7] + 8ULL * buf[k-10];
        buf[k] = pos - neg;
    }
    buf_max = n;
    return buf[n];
}
