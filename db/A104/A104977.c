/*
 * A104977  Defining sequence for an inverse Fredholm-Rueppel triangle.
 * https://oeis.org/A104977
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 13, i16 n <= 26, i32 n <= 53, i64 n <= 106,
 *            i128 n <= 212, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A104977 (inverse Fredholm‑Rueppel triangle)
    Recurrence:
        a(0) = 1
        a(n) = - Σ_{k≥1 : (2^k‑1) ≤ n} a(n - (2^k‑1)),   for n ≥ 1    */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* cache for previously computed values */
    static seq_t *buf  = NULL;
    static idx_t  cap  = 0;
    static idx_t  len  = 0;

    /* enlarge storage if necessary */
    if (n + 1 > cap) {
        idx_t newcap = cap ? cap << 1 : 8;
        while (newcap < n + 1)
            newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(buf, newcap * sizeof(seq_t));
        if (!tmp) abort();          /* out of memory */
        buf = tmp;
        cap = newcap;
    }

    /* compute terms up to n (inclusive) */
    for (idx_t i = len; i <= n; ++i) {
        if (i == 0) {
            buf[0] = (seq_t)1;
            continue;
        }
        seq_t sum = 0;
        /* iterate through powers of two: 2^k-1, k = 1, 2, ... */
        for (idx_t k = 1; ; ++k) {
            idx_t offset = ((idx_t)1 << k) - 1;
            if (offset > i) break;
            sum += buf[i - offset];
        }
        buf[i] = -sum;
    }
    len = n + 1;

    return buf[n];
}
