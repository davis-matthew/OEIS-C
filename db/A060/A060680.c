/*
 * A060680  Smallest difference between consecutive divisors of n.
 * https://oeis.org/A060680
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest difference between consecutive divisors of n
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static int cmp_idx_t(const void *a, const void *b)
{
    idx_t va = *(const idx_t *)a;
    idx_t vb = *(const idx_t *)b;
    return (va < vb) ? -1 : (va > vb);
}

/* Smallest difference between consecutive divisors of n (n >= 2). */
seq_t a(idx_t n)
{
    if (n < 2) return 0; /* undefined below offset */

    /* dynamic array of divisors */
    static idx_t *divs = NULL;
    static size_t div_cap = 0;
    size_t cnt = 0;

    if (divs == NULL) {
        div_cap = 64;
        divs = (idx_t *)malloc(div_cap * sizeof(idx_t));
        if (!divs) return 0; /* allocation failure */
    }

    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            if (cnt >= div_cap) {
                div_cap *= 2;
                divs = (idx_t *)realloc(divs, div_cap * sizeof(idx_t));
                if (!divs) return 0;
            }
            divs[cnt++] = i;
            if (i != n / i) {
                if (cnt >= div_cap) {
                    div_cap *= 2;
                    divs = (idx_t *)realloc(divs, div_cap * sizeof(idx_t));
                    if (!divs) return 0;
                }
                divs[cnt++] = n / i;
            }
        }
    }

    /* sort divisors */
    qsort(divs, cnt, sizeof(idx_t), cmp_idx_t);

    /* compute smallest difference */
    seq_t min_diff = (seq_t)(divs[1] - divs[0]);
    for (size_t i = 1; i+1 < cnt; ++i) {
        seq_t diff = (seq_t)(divs[i+1] - divs[i]);
        if (diff < min_diff) min_diff = diff;
    }
    return min_diff;
}
