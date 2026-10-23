/*
 * A162683  a(0)=1. a(n) = the number of terms among (a(0),a(1),...a(n-1))
 *          that either are coprime to n or divide n.
 * https://oeis.org/A162683
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for n>0, a(n)=count_{k<n} (gcd(a(k),n)==1 || a(k)|n)
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t gcd_seq(seq_t a, seq_t b)          /* Euclidean algorithm for seq_t */
{
    while (b != (seq_t)0) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *terms = NULL;
    static idx_t max_n = 0;           /* highest index already computed */
    static size_t cap   = 0;           /* capacity of the terms array     */

    /* initialise cache on first call */
    if (!terms) {
        const size_t init_cap = 16;
        terms = (seq_t *)malloc(sizeof(seq_t) * init_cap);
        if (!terms) return (seq_t)0;           /* unlikely, but keep safe */
        terms[0] = (seq_t)1;
        cap = init_cap;
        max_n = 0;
    }

    /* compute missing terms up to n */
    if (n > max_n) {
        const size_t need = (size_t)n + 1;
        if (need > cap) {
            size_t new_cap = cap ? cap : 1;
            while (new_cap < need) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(terms, sizeof(seq_t) * new_cap);
            if (!tmp) return (seq_t)0;          /* keep safe on OOM */
            terms = tmp;
            cap = new_cap;
        }

        const seq_t one = (seq_t)1;

        for (idx_t cur = max_n + 1; cur <= n; ++cur) {
            const seq_t cur_seq = (seq_t)cur;
            seq_t cnt = 0;

            for (idx_t k = 0; k < cur; ++k) {
                seq_t g = gcd_seq(terms[k], cur_seq);
                if (g == one || cur_seq % terms[k] == (seq_t)0)
                    ++cnt;
            }

            terms[cur] = cnt;
        }

        max_n = n;
    }

    return terms[n];
}
