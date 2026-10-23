/*
 * A117524  Total number of parts of multiplicity 3 in all partitions of n.
 * https://oeis.org/A117524
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n)=Sum_{k>=1} (p(n-3k)-p(n-4k)) where p(m) is the partition
 *            number (p(0)=1)
 * Range:     u8 n <= 21, u16 n <= 47, u32 n <= 130, u64 n <= 411,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, free */

static idx_t max_n = 0;
static seq_t *part = NULL;   /* partition numbers p[0..max_n]   */
static seq_t *aarr  = NULL;   /* result a[0..max_n] for sequence */

/* compute partition numbers up to n using the recurrence with generalized pentagonal numbers */
static void compute_partitions(idx_t n)
{
    if (part) free(part);
    part = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    part[0] = 1;   /* p(0)=1 */

    for (idx_t i = 1; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t k = 1; ; ++k) {
            idx_t pent1 = k * (3 * k - 1) / 2;
            if (pent1 > i) break;
            idx_t pent2 = k * (3 * k + 1) / 2;
            int sign = (k & 1) ? 1 : -1;   /* (k%2==1) -> +1, else -1 */

            if (sign > 0) {
                sum += part[i - pent1];
                if (pent2 <= i) sum += part[i - pent2];
            } else {
                sum -= part[i - pent1];
                if (pent2 <= i) sum -= part[i - pent2];
            }
        }
        part[i] = sum;
    }
}

/* compute a(n) for all 1..n using the derived formula */
static void compute_a(idx_t n)
{
    if (aarr) free(aarr);
    aarr = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    for (idx_t i = 0; i <= n; ++i) aarr[i] = 0;

    for (idx_t k = 1; 3 * k <= n; ++k) {
        idx_t off1 = 3 * k;
        for (idx_t i = off1; i <= n; ++i) aarr[i] += part[i - off1];

        idx_t off2 = 4 * k;
        for (idx_t i = off2; i <= n; ++i) aarr[i] -= part[i - off2];
    }
}

/* external function required by the build system */
seq_t a(idx_t n)
{
    if (n == 0) return 0;

    if (n > max_n) {
        compute_partitions(n);
        compute_a(n);
        max_n = n;
    }
    return aarr[n];
}
