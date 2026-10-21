/*
 * A224174  Number of 3 X n 0..3 arrays with rows, diagonals and antidiagonals
 *          unimodal and columns nondecreasing.
 * https://oeis.org/A224174
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 12, u64 n <= 55, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A224174 – number of 3×n 0..3 arrays with rows,
   diagonals, antidiagonals unimodal and columns nondecreasing.
   This sequence is a polynomial of degree 18.  The first 21 terms
   determine the 19 forward difference values Δ⁰…Δ¹⁸ a(1).  We
   compute these differences once and then evaluate
   a(n)=∑_{k=0}^{18} Δᵏ a(1)·C(n-1,k).  The function works for any
   n≥1 with O(18) arithmetic operations. */
#include <stdbool.h>

seq_t a(idx_t n)
{
    static bool init_done = false;
    static seq_t delta[19];

    if (!init_done) {
        /* First 21 terms of the sequence (1‑based indices).  They are
           used only to initialise the forward‐difference table. */
        static const seq_t init_terms[22] = {
            0ULL,                    /* unused 0‑th entry */
            20ULL, 400ULL, 4739ULL, 38561ULL,
            242114ULL, 1253770ULL, 5588411ULL,
            22075529ULL, 78911656ULL,
            259222964ULL, 791787532ULL,
            2269535600ULL, 6149706181ULL,
            15847969147ULL, 39036356744ULL,
            92295124070ULL, 210220319550ULL,
            462719488690ULL, 986952548003ULL,
            2044823738533ULL, 4124042889158ULL
        };

        /* Forward difference table.  cur[i] holds the ith element of
           the current difference level; after each level it is
           shifted to compute the next. */
        seq_t cur[22];
        for (idx_t i = 1; i <= 21; ++i)
            cur[i] = init_terms[i];

        for (idx_t k = 0; k < 19; ++k) {
            delta[k] = cur[1];
            /* shift left by one position: cur[i] = cur[i+1] - cur[i] */
            for (idx_t i = 1; i + 1 <= 21 - k; ++i)
                cur[i] = cur[i + 1] - cur[i];
        }
        init_done = true;
    }

    /* Evaluate a(n) using binomial coefficients C(n‑1,k).  The
       recurrence C(n‑1,k+1) = C(n‑1,k)·(n‑1‑k)/(k+1) is used. */
    seq_t result = 0;
    seq_t binom   = 1;          /* C(n‑1,0) */
    result += delta[0] * binom;

    for (idx_t k = 1; k < 19; ++k) {
        binom = binom * (seq_t)(n - k);          /* (n‑1)‑(k‑1) == n‑k */
        binom = binom / (seq_t)k;                /* divide by k */
        result += delta[k] * binom;
    }
    return result;
}
