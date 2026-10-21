/*
 * A200214  Ordered factorizations of n with 3 distinct parts, all > 1.
 * https://oeis.org/A200214
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6 * number of unordered factorizations of n into three
 *            distinct factors all > 1. This is A200214 from OEIS. The
 *            implementation enumerates all divisors of n, then counts triples
 *            a<b<c with a*b*c=n, and multiplies by 6 to account for the 6
 *            orderings
 * Range:     u8 n <= 1001, u16 n <= 1001, u32 n <= 1001, u64 n <= 1001,
 *            u128 n <= 1001, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

static int cmp_idx(const void *p1, const void *p2)
{
    const idx_t *a = (const idx_t *)p1;
    const idx_t *b = (const idx_t *)p2;
    if (*a < *b) return -1;
    if (*a > *b) return 1;
    return 0;
}

seq_t a(idx_t n)
{
    if (n < 24) /* cannot have 3 distinct factors >1 for n<24 */
        return (seq_t)0;

    /* Collect all divisors of n that are >1 */
    idx_t *divs = NULL;
    size_t dcnt = 0, dcap = 0;
    for (idx_t i = 2; i * i <= n; ++i) {
        if (n % i == 0) {
            if (dcap == dcnt) {
                dcap = dcap ? dcap * 2 : 8;
                divs = (idx_t *)realloc(divs, dcap * sizeof(idx_t));
            }
            divs[dcnt++] = i;
            idx_t j = n / i;
            if (j != i) {
                if (dcap == dcnt) {
                    dcap = dcap ? dcap * 2 : 8;
                    divs = (idx_t *)realloc(divs, dcap * sizeof(idx_t));
                }
                divs[dcnt++] = j;
            }
        }
    }
    /* include n itself as a divisor >1 */
    if (dcap == dcnt) {
        dcap = dcap ? dcap * 2 : 8;
        divs = (idx_t *)realloc(divs, dcap * sizeof(idx_t));
    }
    divs[dcnt++] = n;

    /* Sort divisors ascending to make a<b<c easier to enforce */
    qsort(divs, dcnt, sizeof(idx_t), cmp_idx);

    idx_t count = 0;
    for (size_t i = 0; i < dcnt; ++i) {
        idx_t a_val = divs[i];
        for (size_t j = i + 1; j < dcnt; ++j) {
            idx_t b_val = divs[j];
            if (a_val * b_val > n) /* cannot divide further */
                break;
            if (n % (a_val * b_val) != 0)
                continue;
            idx_t c_val = n / (a_val * b_val);
            if (c_val > b_val) /* ensures a<b<c distinct */
                ++count;
        }
    }

    free(divs);
    return (seq_t)6 * (seq_t)count;
}
