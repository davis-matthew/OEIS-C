/*
 * A234562  Number of (n+1) X (6+1) 0..4 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 4
 *          (constant-stress 1 X 1 tilings).
 * https://oeis.org/A234562
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4, u32 n <= 17, u64 n <= 36, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 4*a(n-1) + 30*a(n-2) -132*a(n-3) -331*a(n-4)
 *                +1720*a(n-5) +1530*a(n-6) -11280*a(n-7) -1324*a(n-8)
 *                +39136*a(n-9) -12360*a(n-10) -67968*a(n-11)
 *                +39456*a(n-12) +46080*a(n-13) -34560*a(n-14)
 *   for n ≥ 15, with the first 14 terms given below. */
#include <stddef.h>

static const int COEF[14] = {
    4,      30,   -132,  -331,
    1720,   1530, -11280, -1324,
    39136,  -12360, -67968, 39456,
    46080,  -34560
};

static const seq_t INIT[15] = {
    0ULL,           /* dummy for index 0 */
    17518ULL,       /* a(1) */
    24420ULL,       /* a(2) */
    33946ULL,       /* a(3) */
    57210ULL,       /* a(4) */
    94114ULL,       /* a(5) */
    187686ULL,      /* a(6) */
    358450ULL,      /* a(7) */
    810702ULL,      /* a(8) */
    1742578ULL,     /* a(9) */
    4339470ULL,     /* a(10) */
    10208626ULL,    /* a(11) */
    27473790ULL,    /* a(12) */
    69051154ULL,    /* a(13) */
    197748366ULL    /* a(14) */
};

seq_t a(idx_t n)
{
    /* For indices lower than the offset, return 0. */
    if (n < 1) return (seq_t)0;
    /* Return known initial terms directly. */
    if (n <= 14) return INIT[(size_t)n];

    /* Buffer holding the last 14 computed values:
       buf[0] = a(k-1), buf[1] = a(k-2), …, buf[13] = a(k-14). */
    seq_t buf[14];
    for (size_t i = 0; i < 14; ++i)
        buf[i] = INIT[14 - i];  /* start with a(14), a(13), …, a(1) */

    /* Compute terms up to the requested index. */
    for (idx_t k = 15; k <= n; ++k) {
        seq_t pos = 0, neg = 0;
        for (int j = 0; j < 14; ++j) {
            if (COEF[j] >= 0)
                pos += (seq_t)COEF[j] * buf[j];
            else
                neg += (seq_t)(-COEF[j]) * buf[j];
        }
        seq_t cur = pos - neg;  /* non‑negative by definition */

        /* Shift buffer left and insert the new value. */
        for (size_t j = 13; j > 0; --j)
            buf[j] = buf[j - 1];
        buf[0] = cur;
    }

    return buf[0];
}
