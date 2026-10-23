/*
 * A018736  Divisors of 945.
 * https://oeis.org/A018736
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition: a(n) is the n‑th smallest divisor of 945 (sorted ascending). */
#include <stdlib.h>

static int div_cmp(const void *a, const void *b)
{
    const seq_t *va = (const seq_t *)a;
    const seq_t *vb = (const seq_t *)b;
    if (*va < *vb) return -1;
    if (*va > *vb) return 1;
    return 0;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1 */

    static seq_t *divs = NULL;
    static size_t divcnt = 0;

    if (divs == NULL) {
        /* 945 = 3^3 · 5^1 · 7^1  →  4·2·2 = 16 divisors */
        const int primes[3] = {3, 5, 7};
        const int exps[3]   = {3, 1, 1};

        size_t total = 1;
        for (int i = 0; i < 3; ++i)
            total *= (size_t)(exps[i] + 1);

        divs = (seq_t *)malloc(total * sizeof(seq_t));
        if (divs == NULL)
            return 0;              /* allocation failure */

        size_t pos = 0;
        for (int e0 = 0; e0 <= exps[0]; ++e0) {
            seq_t p0 = 1;
            for (int i = 0; i < e0; ++i)
                p0 *= (seq_t)primes[0];
            for (int e1 = 0; e1 <= exps[1]; ++e1) {
                seq_t p1 = p0;
                for (int i = 0; i < e1; ++i)
                    p1 *= (seq_t)primes[1];
                for (int e2 = 0; e2 <= exps[2]; ++e2) {
                    seq_t p2 = p1;
                    for (int i = 0; i < e2; ++i)
                        p2 *= (seq_t)primes[2];
                    divs[pos++] = p2;
                }
            }
        }

        qsort(divs, total, sizeof(seq_t), div_cmp);
        divcnt = total;
    }

    if (n > divcnt) return 0;
    return divs[(size_t)(n - 1)];
}
