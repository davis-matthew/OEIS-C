/*
 * A208477  Difference between the sum of odd parts and the sum of even parts
 *          in all the partitions of n.
 * https://oeis.org/A208477
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*∑_{odd k≤n} k * ∑_{m≥1, k*m≤n} p(n−k*m) − n·p(n) ,
 *            where p(k) is the partition function. This follows from a(n) =
 *            2·SumOdd(n) – n·p(n) and the identity SumOdd(n) = ∑_{odd k} k ·
 *            #occurrences of part k in all partitions of n
 * Range:     u8 n <= 14, u16 n <= 38, u32 n <= 114, u64 n <= 383,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* ----------  partition numbers p[0..n]  ---------- */
    static seq_t *p   = NULL;
    static idx_t p_max = 0;

    /* allocate p[0] if this is the first call */
    if (!p) {
        p = (seq_t*)malloc(sizeof(seq_t));   /* p[0] only for now */
        if (!p) return 0;                    /* out‑of‑memory guard */
        p[0] = (seq_t)1;                     /* p(0) = 1 */
        p_max = 0;
    }

    /* extend the table up to n */
    if (n > p_max) {
        seq_t *new_p = (seq_t*)realloc(p, (n + 1) * sizeof(seq_t));
        if (!new_p) return 0;                /* out‑of‑memory guard */
        p = new_p;

        for (idx_t i = p_max + 1; i <= n; ++i) {
            seq_t pos = (seq_t)0;            /* sum of terms with positive sign   */
            seq_t neg = (seq_t)0;            /* sum of terms with negative sign   */

            /* Euler's pentagonal number theorem */
            for (idx_t k = 1; ; ++k) {
                idx_t pent1 = k * (3 * k - 1) / 2;
                if (pent1 > i) break;          /* further pentagonal numbers are larger */

                seq_t add = p[i - pent1];
                if (k & 1) pos += add; else neg += add;

                idx_t pent2 = k * (3 * k + 1) / 2;
                if (pent2 > i) continue;
                if (k & 1) pos += p[i - pent2]; else neg += p[i - pent2];
            }

            p[i] = pos - neg;                 /* pos ≥ neg for all i */
        }
        p_max = n;
    }

    /* ----------  compute a(n)  ---------- */
    seq_t sumOdd = (seq_t)0;

    for (idx_t k = 1; k <= n; k += 2) {      /* only odd k contribute positively   */
        seq_t occ = (seq_t)0;
        for (idx_t t = k; t <= n; t += k) {
            occ += p[n - t];                /* sum of p[n - m*k] over m ≥ 1       */
        }
        sumOdd += (seq_t)k * occ;          /* add k * occ to SumOdd               */
    }

    seq_t twoSumOdd = sumOdd + sumOdd;      /* 2 * SumOdd                         */
    seq_t np        = (seq_t)n * p[n];      /* n · p(n)                           */
    seq_t result    = twoSumOdd - np;       /* difference – guaranteed non‑negative */

    return result;
}
