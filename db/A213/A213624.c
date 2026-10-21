/*
 * A213624  Expansion of psi(x)^2 * psi(x^4) in powers of x where psi() is a
 *          Ramanujan theta function.
 * https://oeis.org/A213624
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the Euler transform of the period‑8 sequence [2, -2, 2,
 *            -1, 2, -2, 2, -3, ...]. It satisfies the recurrence a(0) = 1,
 *            a(n) = (1/n) * Σ_{k=1..n} (c(k) * a(n-k)), n ≥ 1, where c(k) =
 *            Σ_{d|k} d * b(d) and b(d) is the periodic term (2, -2, 2, -1, 2,
 *            -2, 2, -3)
 * Range:     u8 n <= 27, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Special case n = 0 */
    if (n == 0) return (seq_t)1;

    /* Convert index to a native size for array bounds */
    size_t N = (size_t)n;

    /* Periodic sequence b(d) */
    static const int8_t b_pattern[8] = { 2, -2, 2, -1, 2, -2, 2, -3 };

    /* Allocate array for c(k) = Σ_{d|k} d*b(d) */
    int64_t *c = (int64_t *)calloc(N + 1, sizeof(int64_t));
    if (!c) return (seq_t)0;   /* fallback on allocation failure */

    /* Precompute c(k) */
    for (size_t d = 1; d <= N; ++d) {
        int64_t bd = (int64_t)b_pattern[(d - 1) % 8];
        if (bd == 0) continue;   /* not needed but keeps logic clear */
        for (size_t k = d; k <= N; k += d)
            c[k] += (int64_t)d * bd;
    }

    /* Allocate array for the sequence a */
    seq_t *a_arr = (seq_t *)calloc(N + 1, sizeof(seq_t));
    if (!a_arr) { free(c); return (seq_t)0; }

    a_arr[0] = (seq_t)1;   /* initial value */

    /* Compute a(n) via the recurrence */
    for (size_t cur = 1; cur <= N; ++cur) {
        seq_t sum_pos = 0;
        seq_t sum_neg = 0;

        for (size_t k = 1; k <= cur; ++k) {
            int64_t ck = c[k];
            if (ck == 0) continue;
            seq_t term = a_arr[cur - k] *
                         (seq_t)(ck >= 0 ? ck : -ck);
            if (ck >= 0)
                sum_pos += term;
            else
                sum_neg += term;
        }

        /* diff = sum_pos - sum_neg  (guaranteed non‑negative) */
        seq_t diff = sum_pos - sum_neg;
        a_arr[cur] = diff / (seq_t)cur;
    }

    seq_t result = a_arr[N];

    free(c);
    free(a_arr);
    return result;
}
