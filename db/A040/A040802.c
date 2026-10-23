/*
 * A040802  Continued fraction for sqrt(831).
 * https://oeis.org/A040802
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(n) are the terms of the continued‑fraction expansion of
 *            sqrt(831). a(0) = floor(sqrt(831)) = 28. For n>0, a(n) is the
 *            n‑th term of the periodic part (the last term of a period equals
 *            2*a0)
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static seq_t *terms = NULL;
    static size_t period = 0;
    static seq_t a0 = 0;

    if (terms == NULL) {
        /* Compute a0 = floor(sqrt(831)) using 64‑bit arithmetic. */
        uint64_t tmp_a0 = 0;
        while ((tmp_a0 + 1) * (tmp_a0 + 1) <= 831ULL)
            ++tmp_a0;
        a0 = (seq_t)tmp_a0;

        /* Continued‑fraction expansion of sqrt(831). */
        const uint64_t N = 831ULL;
        uint64_t m = 0;
        uint64_t d = 1;
        uint64_t a = tmp_a0;

        size_t cap = 16;
        terms = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!terms) return 0;          /* allocation failed */

        size_t len = 0;
        /* Generate one period; the last term equals 2*a0. */
        do {
            m = d * a - m;
            uint64_t dm = N - m * m;
            d = dm / d;
            a = (tmp_a0 + m) / d;

            if (len == cap) {
                cap *= 2;
                seq_t *tmp = (seq_t*)realloc(terms, cap * sizeof(seq_t));
                if (!tmp) return 0;
                terms = tmp;
            }
            terms[len++] = (seq_t)a;
        } while (a != 2 * tmp_a0);

        period = len;
    }

    if (n == 0) {
        return a0;
    }
    /* n > 0 */
    size_t idx = (size_t)((n - 1) % (idx_t)period);
    return terms[idx];
}
