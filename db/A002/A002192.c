/*
 * A002192  Least integer with A000203(a(n)) = A002191(n), where A002191 =
 *          range of the sum-of-divisors function A000203.
 * https://oeis.org/A002192
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   search by sieve. Compute sigma(m) for all m <= L with a
 *            divisor-sum sieve. Since sigma(m) > m for m > 1, every m with
 *            sigma(m) <= L is also <= L, so the set of sigma values up to L
 *            is complete. Record the least m for each value v <= L. Then a(n)
 *            is the least m for the n-th smallest value, i.e. A0...
 * Range:     u8 n <= 94, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint64_t *a2192_res = NULL;
static idx_t a2192_cnt = 0;

static void a2192_build(idx_t need) {
    uint64_t L = 1024;
    while ((uint64_t)need * 3 > L) L *= 2;
    for (;;) {
        uint64_t *sg = (uint64_t *)calloc((size_t)(L + 1), sizeof(uint64_t));
        uint64_t *mn = (uint64_t *)calloc((size_t)(L + 1), sizeof(uint64_t));
        uint64_t d, k, m, v;
        idx_t c = 0;
        for (d = 1; d <= L; d++)
            for (k = d; k <= L; k += d)
                sg[k] += d;
        for (m = 1; m <= L; m++) {
            v = sg[m];
            if (v <= L && mn[v] == 0) mn[v] = m;
        }
        for (v = 1; v <= L; v++)
            if (mn[v]) c++;
        if (c >= need) {
            uint64_t *r = (uint64_t *)malloc((size_t)c * sizeof(uint64_t));
            idx_t i = 0;
            for (v = 1; v <= L; v++)
                if (mn[v]) r[i++] = mn[v];
            free(a2192_res);
            a2192_res = r;
            a2192_cnt = c;
            free(sg);
            free(mn);
            return;
        }
        free(sg);
        free(mn);
        L *= 2;
    }
}

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;
    if (n > a2192_cnt) a2192_build(n);
    return (seq_t)a2192_res[n - 1];
}
