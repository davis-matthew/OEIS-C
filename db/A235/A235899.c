/*
 * A235899  Number of (n+1)X(5+1) 0..2 arrays with the minimum plus the
 *          maximum equal to the lower median plus the upper median in every
 *          2X2 subblock.
 * https://oeis.org/A235899
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 49*a(n-1) -680*a(n-2) -2152*a(n-3) +106896*a(n-4)
 *            -244848*a(n-5) -6547072*a(n-6) +18222592*a(n-7)
 *            +219136000*a(n-8) -245395456*a(n-9) -3590750208*a(n-10)
 *            -5119672320*a(n-11), with a(1)=6849, a(2)=89857, a(3)=1030145,
 *            a(4)=12931393, a(5)=160019457, a(6)=2053474753,
 *            a(7)=26681279873, a(8)=35...
 * Range:     u8 none, u16 n <= 1, u32 n <= 6, u64 n <= 14, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* If n is below the sequence offset, the value is undefined. */
    if (n < 1ULL) return 0;

    /* Sequence values for n = 1 .. 11.  Already stored as seq_t. */
    static const seq_t init[11] = {
        (seq_t)6849ULL,
        (seq_t)89857ULL,
        (seq_t)1030145ULL,
        (seq_t)12931393ULL,
        (seq_t)160019457ULL,
        (seq_t)2053474753ULL,
        (seq_t)26681279873ULL,
        (seq_t)354762714433ULL,
        (seq_t)4797433339137ULL,
        (seq_t)66057474247873ULL,
        (seq_t)923795771239553ULL
    };

    /* Coefficients of the 11-term linear recurrence.  Signed long long
       is large enough to hold each coefficient. */
    static const long long coeff[11] = {
        49LL,          -680LL,       -2152LL,         106896LL,
        -244848LL,   -6547072LL,     18222592LL,     219136000LL,
        -245395456LL, -3590750208LL, -5119672320LL
    };

    /* Return one of the first 11 precomputed terms. */
    if (n <= 11ULL) {
        return init[(size_t)(n - 1ULL)];
    }

    /* Circular buffer holding the last 11 computed values:
         last[0] = a(k-1), last[1] = a(k-2), ..., last[10] = a(k-11) */
    seq_t last[11];
    for (int i = 0; i < 11; ++i) {
        last[i] = init[10 - i];
    }

    seq_t result = 0;

    for (idx_t k = 12ULL; k <= n; ++k) {
        seq_t pos_sum = 0;
        seq_t neg_sum = 0;

        for (int i = 0; i < 11; ++i) {
            unsigned long long absCoeff = coeff[i] >= 0
                ? (unsigned long long)coeff[i]
                : (unsigned long long)(-coeff[i]);

            seq_t prod = last[i] * (seq_t)absCoeff;

            if (coeff[i] >= 0) {
                pos_sum += prod;
            } else {
                neg_sum += prod;
            }
        }

        result = pos_sum - neg_sum;   /* recurrence guarantees non-negative */

        /* Shift last[ ] to make room for the new term. */
        for (int i = 10; i >= 1; --i) {
            last[i] = last[i - 1];
        }
        last[0] = result;
    }

    return result;
}
