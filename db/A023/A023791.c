/*
 * A023791  Katadromes: digits in base 9 are in strict descending order.
 * https://oeis.org/A023791
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full, easy
 * Formula:   a(n) = the n-th smallest non‑negative integer whose base‑9
 *            representation has strictly descending digits
 * Range:     u8 n <= 2, u16 n <= 255, u32 n <= 511, u64 n <= 511,
 *            u128 n <= 511, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for qsort */

static int cmp_seq(const void *p, const void *q)
{
    seq_t v1 = *(const seq_t *)p;
    seq_t v2 = *(const seq_t *)q;
    if (v1 < v2) return -1;
    if (v1 > v2) return 1;
    return 0;
}

seq_t a(idx_t n)
{
    static seq_t vals[512];
    static size_t cnt = 0;
    if (cnt == 0) {
        /* generate all numbers with strictly decreasing base‑9 digits */
        const int DIGITS = 9;            /* digits 0…8 */
        seq_t pow9[9];
        pow9[0] = (seq_t)1;
        for (int i = 1; i < DIGITS; ++i)
            pow9[i] = pow9[i-1] * (seq_t)9;

        size_t idx = 0;
        /* non‑empty subsets of {0,…,8} produce numbers;
           mask 0 would give an empty number, skipped. */
        for (int mask = 1; mask < (1 << DIGITS); ++mask) {
            seq_t val = 0;
            seq_t mult = 1;
            for (int d = 0; d < DIGITS; ++d) {
                if ((mask >> d) & 1) {
                    val += (seq_t)d * mult;
                    mult *= 9;
                }
            }
            vals[idx++] = val;
        }
        cnt = idx;                 /* should be 511 */
        qsort(vals, cnt, sizeof(seq_t), cmp_seq);
    }

    if (n < 1 || n > cnt)      /* out of range */
        return (seq_t)0;

    return vals[n-1];
}
