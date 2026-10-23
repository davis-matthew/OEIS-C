/*
 * A190162  Number of peakless Motzkin paths of length n containing no
 *          subwords of type dh^ju (j>=1), where u=(1,1), h=(1,0), and
 *          d=(1,-1) (can be easily expressed using RNA secondary structure
 *          terminology).
 * https://oeis.org/A190162
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the recurrence * (n+2)a(n) –5(n+1)a(n‑1) +
 *            (8n+2)a(n‑2) + (‑6n+5)a(n‑3) + (8n‑27)a(n‑4) * + (‑14n+62)a(n‑5)
 *            + (13n‑71)a(n‑6) + (‑7n+47)a(n‑7) + (3n‑25)a(n‑8) * +
 *            (‑n+9)a(n‑9) = 0 for n ≥ 9 * * Initial values: a(0)=1, a(1)=1,
 *            a(2)=1, a(3)=2, a(4)=4, * a(5)=8, a(6)=17, a(7)=36, a(8)=77
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 25, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    static seq_t *buf = NULL;
    static size_t cap = 0;
    static idx_t len = 0;

    /* grow the buffer if needed */
    if ((size_t)n >= cap) {
        size_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(*tmp));
        if (!tmp) return 0; /* allocation failure */
        if (buf) {
            memcpy(tmp, buf, cap * sizeof(*buf));
            free(buf);
        }
        buf = tmp;
        cap = new_cap;
    }

    /* compute values up to n */
    for (idx_t i = len; i <= n; ++i) {
        if (i < 9) {
            static const seq_t init[9] = {1, 1, 1, 2, 4, 8, 17, 36, 77};
            buf[i] = init[(size_t)i];
        } else {
            /* split the sum into positive and negative parts to avoid signed overflow */
            seq_t sumPos = 0;
            seq_t sumNeg = 0;

            /* a(i-1) coefficient: -5(i+1)  (always negative for i≥1) */
            int c1 = -(5 * (i + 1));
            if (c1 > 0)
                sumPos += mul_small(buf[i - 1], (unsigned int)c1);
            else if (c1 < 0)
                sumNeg += mul_small(buf[i - 1], (unsigned int)(-c1));

            /* a(i-2) coefficient: 8i+2   (positive) */
            int c2 = 8 * i + 2;
            sumPos += mul_small(buf[i - 2], (unsigned int)c2);

            /* a(i-3) coefficient: -6i+5  (negative for i≥1) */
            int c3 = -6 * i + 5;
            if (c3 > 0)
                sumPos += mul_small(buf[i - 3], (unsigned int)c3);
            else if (c3 < 0)
                sumNeg += mul_small(buf[i - 3], (unsigned int)(-c3));

            /* a(i-4) coefficient: 8i-27  (positive for i≥4) */
            int c4 = 8 * i - 27;
            if (c4 > 0)
                sumPos += mul_small(buf[i - 4], (unsigned int)c4);
            else if (c4 < 0)
                sumNeg += mul_small(buf[i - 4], (unsigned int)(-c4));

            /* a(i-5) coefficient: -14i+62 (negative for i≥5) */
            int c5 = -14 * i + 62;
            if (c5 > 0)
                sumPos += mul_small(buf[i - 5], (unsigned int)c5);
            else if (c5 < 0)
                sumNeg += mul_small(buf[i - 5], (unsigned int)(-c5));

            /* a(i-6) coefficient: 13i-71  (negative for i≥6) */
            int c6 = 13 * i - 71;
            if (c6 > 0)
                sumPos += mul_small(buf[i - 6], (unsigned int)c6);
            else if (c6 < 0)
                sumNeg += mul_small(buf[i - 6], (unsigned int)(-c6));

            /* a(i-7) coefficient: -7i+47  (negative for i≥7) */
            int c7 = -7 * i + 47;
            if (c7 > 0)
                sumPos += mul_small(buf[i - 7], (unsigned int)c7);
            else if (c7 < 0)
                sumNeg += mul_small(buf[i - 7], (unsigned int)(-c7));

            /* a(i-8) coefficient: 3i-25   (negative for i≥9) */
            int c8 = 3 * i - 25;
            if (c8 > 0)
                sumPos += mul_small(buf[i - 8], (unsigned int)c8);
            else if (c8 < 0)
                sumNeg += mul_small(buf[i - 8], (unsigned int)(-c8));

            /* a(i-9) coefficient: -i+9   (positive until i=8, negative thereafter) */
            int c9 = -i + 9;
            if (c9 > 0)
                sumPos += mul_small(buf[i - 9], (unsigned int)c9);
            else if (c9 < 0)
                sumNeg += mul_small(buf[i - 9], (unsigned int)(-c9));

            /* compute a(i) = (sumNeg - sumPos) / (i+2) */
            seq_t diff = sumNeg - sumPos;
            buf[i] = diff / (seq_t)(i + 2);
        }
    }

    len = n + 1;
    return buf[n];
}
