/*
 * A061185  One half of sixth (m=5) column of triangle A060921 (bisection of
 *          Fibonacci triangle, odd part).
 * https://oeis.org/A061185
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A060921(n+5,5)/2, recurrence from g.f. (1-3x+x^2)^6
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 12, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) satisfies: a(n) = 18 a(n-1) -141 a(n-2) +630 a(n-3) -1770 a(n-4)
 *     +3258 a(n-5) -3989 a(n-6) +3258 a(n-7) -1770 a(n-8) +630 a(n-9)
 *     -141 a(n-10) +18 a(n-11) - a(n-12).  The first 12 terms are
 *     a(0)=3, a(1)=49, a(2)=462, a(3)=3294, a(4)=19715, a(5)=104517,
 *     a(6)=506646, a(7)=2292310, a(8)=9817920, a(9)=40210800,
 *     a(10)=158677370, a(11)=606790410. */

#include <stdlib.h>

static const int init_vals[] = {
    3, 49, 462, 3294, 19715, 104517,
    506646, 2292310, 9817920, 40210800,
    158677370, 606790410
};

static const int coeff[] = {
    0,   /* placeholder for index 0 */
    18,  -141, 630, -1770, 3258,
    -3989, 3258, -1770, 630, -141,
    18, -1
};

seq_t a(idx_t n)
{
    static seq_t *terms = NULL;
    static size_t terms_len = 0;

    /* if n < 12 we can return stored term after ensuring initial terms */
    if (terms_len == 0) {
        size_t init_len = sizeof(init_vals) / sizeof(init_vals[0]);
        terms_len = init_len;
        terms = (seq_t *)malloc(terms_len * sizeof(seq_t));
        for (size_t i = 0; i < init_len; ++i)
            terms[i] = (seq_t) init_vals[i];
    }

    /* grow the table if needed */
    if ((size_t)n + 1 > terms_len) {
        size_t new_len = (size_t)n + 1;
        seq_t *new_terms = (seq_t *)realloc(terms, new_len * sizeof(seq_t));
        if (new_terms == NULL) {
            /* allocation failed – in competitive programming context abort */
            abort();
        }
        terms = new_terms;

        /* fill any new initial entries that were not set before */
        size_t init_len = sizeof(init_vals) / sizeof(init_vals[0]);
        for (size_t i = terms_len; i < new_len && i < init_len; ++i)
            terms[i] = (seq_t) init_vals[i];

        /* compute remaining terms using recurrence */
        size_t start = (terms_len > 12) ? terms_len : 12;
        if (start < 12 && new_len > 12)
            start = 12;
        for (size_t i = start; i < new_len; ++i) {
            seq_t val = 0;
            for (int k = 1; k <= 12; ++k) {
                seq_t t = terms[i - k];
                int c = coeff[k];
                if (c >= 0)
                    val = val + (seq_t)c * t;
                else
                    val = val - (seq_t)(-c) * t;
            }
            terms[i] = val;
        }
        terms_len = new_len;
    }

    return terms[n];
}
