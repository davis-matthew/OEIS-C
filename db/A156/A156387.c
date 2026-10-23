/*
 * A156387  Number of 5X5 arrays of squares of integers, symmetric under 90
 *          degree rotation, with all rows summing to n.
 * https://oeis.org/A156387
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 5×5 arrays of perfect squares that are *
 *            symmetric under 90° rotation and have all rows summing to n. *
 *            Computation is performed by enumerating the 7 orbit values *
 *            a,b,c,d,e,f,g that satisfy the 3 linear equations: * 2a + b + c
 *            + f = n * f + 2d + e + c = n * 2b + 2e + g = n * Th...
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* small helper: all integers are unsigned, so use uint64_t for calc */
    uint64_t N = n;

    /* --------------------- build list of square values <= n --------------------- */
    /* linear scan is fine for n <= 10000 */
    uint64_t sqs[128];      /* enough for sqrt(10000)=100+ */
    size_t sqCnt = 0;
    for (uint64_t k = 0;; ++k) {
        uint64_t s = k * k;
        if (s > N) break;
        sqs[sqCnt++] = s;
    }

    /* --------------------- build table of perfect squares --------------------- */
    bool *is_sq = (bool *)malloc((size_t)(N + 1) * sizeof(bool));
    if (!is_sq) return 0;     /* allocation failure: return 0 */
    for (size_t i = 0; i <= N; ++i) is_sq[i] = false;
    for (size_t i = 0; i < sqCnt; ++i)
        is_sq[(size_t)sqs[i]] = true;

    /* --------------------- precompute pair sums of square values --------------------- */
    seq_t *pairSum = (seq_t *)calloc((size_t)(N + 1), sizeof(seq_t));
    if (!pairSum) { free(is_sq); return 0; }
    for (size_t i = 0; i < sqCnt; ++i) {
        for (size_t j = 0; j < sqCnt; ++j) {
            uint64_t add = sqs[i] + sqs[j];
            if (add <= N) {
                pairSum[(size_t)add] += 1;
            }
        }
    }

    /* --------------------- enumeration of orbit variables --------------------- */
    seq_t count = 0;
    for (size_t ai = 0; ai < sqCnt; ++ai) {
        uint64_t a = sqs[ai];
        for (size_t bi = 0; bi < sqCnt; ++bi) {
            uint64_t b = sqs[bi];
            uint64_t sum2a_b = 2ULL * a + b;       /* 2a + b   */
            for (size_t ei = 0; ei < sqCnt; ++ei) {
                uint64_t e = sqs[ei];
                if (sum2a_b < e) continue;        /* temp negative */
                uint64_t temp = sum2a_b - e;      /* 2a + b - e */
                if (temp & 1ULL) continue;        /* not even -> d not integer */
                uint64_t d = temp >> 1;           /* d = (2a + b - e)/2 */
                if (d > N || !is_sq[(size_t)d]) continue;

                uint64_t g = N - 2ULL * b - 2ULL * e;
                if (g > N || !is_sq[(size_t)g]) continue;

                if (N < 2ULL * a + b) continue;   /* sum0 negative */
                uint64_t sum0 = N - 2ULL * a - b; /* c + f */
                count += pairSum[(size_t)sum0];
            }
        }
    }

    /* free allocated memory */
    free(is_sq);
    free(pairSum);

    return count;
}
