/*
 * A006933  'Eban' numbers (the letter 'e' is banned!).
 * https://oeis.org/A006933
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  word, nonn
 * Formula:   write n in base 20; digit d of weight 20^k maps to the k-th
 *            allowed three-digit group G[d] in {0,2,4,6,30,32,...,66}, i.e.
 *            a(n) = Sum G[d_k]*1000^p_k, where p_k are the group positions
 *            whose names have no 'e' (units, thousand, million, billion,
 *            trillion, quadrillion, quintillion, octillion, nonil...
 * Range:     u8 n <= 19, u16 n <= 379, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

static const int A006933_G[20] = {
    0, 2, 4, 6,
    30, 32, 34, 36,
    40, 42, 44, 46,
    50, 52, 54, 56,
    60, 62, 64, 66
};

/* powers of 1000 whose -illion names contain no 'e' (American naming) */
static const int A006933_POS[9] = {0, 1, 2, 3, 4, 5, 6, 9, 10};

seq_t a(idx_t n) {
    seq_t result = 0;
    idx_t m = n;
    int k = 0;
    while (m > 0 && k < 9) {
        int d = (int)(m % 20);
        m /= 20;
        if (d != 0) {
            seq_t term = (seq_t)A006933_G[d];
            int p;
            for (p = 0; p < A006933_POS[k]; p++) term *= 1000;
            result += term;
        }
        k++;
    }
    return result;
}
