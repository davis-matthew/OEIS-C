/*
 * A224010  Number of n X 6 0..2 arrays with rows nondecreasing and
 *          antidiagonals unimodal.
 * https://oeis.org/A224010
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Empirical recurrence of order 14 a(n) = 28*a(n‑1) –283*a(n‑2)
 *            +1386*a(n‑3) –3562*a(n‑4) +4940*a(n‑5) –3840*a(n‑6)
 *            –14256*a(n‑7) +50007*a(n‑8) –26386*a(n‑9) +141154*a(n‑10)
 *            +250789*a(n‑11) +41356*a(n‑12) +274344*a(n‑13) +134400*a(n‑14)
 *            Initial values (a(1) = 28, …, a(14) = 37967141667302902)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const seq_t init_terms[14] = {
    (seq_t)28ull,            (seq_t)784ull,            (seq_t)15340ull,
    (seq_t)245055ull,        (seq_t)3516324ull,        (seq_t)47735665ull,
    (seq_t)630339756ull,     (seq_t)8213689391ull,     (seq_t)106375878027ull,
    (seq_t)1373916879120ull, (seq_t)17723771318006ull, (seq_t)228519337975633ull,
    (seq_t)2945698436819118ull, (seq_t)37967141667302902ull
};

static const int64_t coeff[14] = {
    28LL,     -283LL,    1386LL,   -3562LL,
    4940LL,   -3840LL,   -14256LL, 50007LL,
    -26386LL, 141154LL,  250789LL, 41356LL,
    274344LL, 134400LL
};

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset 1 – undefined for 0 */
    if (n <= 14) return init_terms[n - 1];

    /* sliding window of the last 14 terms:
       arr[0] = a(i‑14), arr[13] = a(i‑1) */
    seq_t arr[14];
    for (int i = 0; i < 14; ++i) arr[i] = init_terms[i];

    for (idx_t i = 15; i <= n; ++i) {
        seq_t next = (seq_t)0;
        for (int k = 0; k < 14; ++k) {
            seq_t prod =
                (seq_t)((coeff[k] < 0) ? -coeff[k] : coeff[k]) * arr[13 - k];
            if (coeff[k] >= 0)
                next += prod;
            else
                next -= prod;
        }
        /* shift the window left by one position */
        for (int j = 0; j < 13; ++j) arr[j] = arr[j + 1];
        arr[13] = next;
    }
    return arr[13];
}
