/*
 * A221825  Number of nX3 arrays of occupancy after each element moves to some
 *          horizontal, vertical or antidiagonal neighbor, without 2-loops or
 *          left turns.
 * https://oeis.org/A221825
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A221825
 *  Empirical linear recurrence of order 17:
 *    a(n) = 17·a(n‑1) – 39·a(n‑2) – 14·a(n‑3) + 134·a(n‑4)
 *          – 161·a(n‑5) + 50·a(n‑6) – 147·a(n‑7) + 129·a(n‑8)
 *          + 215·a(n‑9) – 131·a(n‑10) + 53·a(n‑11) – 92·a(n‑12)
 *          – 97·a(n‑13) + 31·a(n‑14) – 27·a(n‑15) + 16·a(n‑16)
 *          + 6·a(n‑17)
 *  with initial terms
 *      a(1)=0, a(2)=16, a(3)=202, a(4)=2930, a(5)=41618,
 *      a(6)=592601, a(7)=8434490, a(8)=120053263, a(9)=1708776751,
 *      a(10)=24321871624, a(11)=346185271087, a(12)=4927426856495,
 *      a(13)=70134512879660, a(14)=998259343308353,
 *      a(15)=14208720864088015, a(16)=202239778618252032,
 *      a(17)=2878579180120974241.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Pre‑computed first 17 terms */
    static const seq_t init[17] = {
        0ULL, 16ULL, 202ULL, 2930ULL, 41618ULL,
        592601ULL, 8434490ULL, 120053263ULL, 1708776751ULL,
        24321871624ULL, 346185271087ULL, 4927426856495ULL,
        70134512879660ULL, 998259343308353ULL, 14208720864088015ULL,
        202239778618252032ULL, 2878579180120974241ULL
    };

    /* Recurrence coefficients for a(n) in descending order
     * (coefficient for a(n-1) first).
     */
    static const int coef[17] = {
        17, -39, -14, 134, -161, 50, -147, 129,
        215, -131, 53, -92, -97, 31, -27, 16, 6
    };

    if (n <= 17) {
        return init[n-1];
    }

    seq_t terms[17];
    size_t i;
    for (i = 0; i < 17; ++i) {
        terms[i] = init[i];
    }

    idx_t k;
    for (k = 18; k <= n; ++k) {
        seq_t sum_pos = 0;
        seq_t sum_neg = 0;
        for (i = 0; i < 17; ++i) {
            seq_t t = terms[16 - i];   /* t = a(k-i-1) */
            if (coef[i] > 0) {
                sum_pos += t * (seq_t)coef[i];
            } else {
                sum_neg += t * (seq_t)(-coef[i]);
            }
        }
        seq_t new_val = sum_pos - sum_neg;

        /* shift terms left, drop a(k-17), insert new_val as a(k) */
        for (i = 0; i < 16; ++i) {
            terms[i] = terms[i + 1];
        }
        terms[16] = new_val;
    }

    return terms[16];
}
