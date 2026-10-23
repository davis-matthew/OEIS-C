/*
 * A035882  Coordination sequence for diamond structure D^+_12. (Edges defined
 *          by l_1 norm = 1.)
 * https://oeis.org/A035882
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 17, u64 n <= 125, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A035882 – Coordination sequence for diamond structure D+12
 *
 *  a(n) satisfies the linear recurrence
 *
 *      a(n) = 12 a(n-2) - 66 a(n-4) + 220 a(n-6) - 495 a(n-8)
 *             + 792 a(n-10) - 924 a(n-12) + 792 a(n-14)
 *             - 495 a(n-16) + 220 a(n-18) - 66 a(n-20)
 *             + 12 a(n-22) - a(n-24) + N(n)
 *
 *  where N(n) is the numerator coefficient of xⁿ in the generating
 *  function and is 0 for n > 24.  N(n) equals
 *
 *    index :  0  1   2  3   4 5   6  7   8   9  10  11  12 13 14 15 16 17 18 19 20 21 22 23 24
 *    value :  1  0 276  0 10626 0 136644 24576 870639 450560 2975016 1622016 4596508 1622016 2975016
 *             450560 870639 24576 136644 0 10626 0 276 0 1
 *
 *  The function below computes a(n) for any n ≥ 0 using only integer
 *  arithmetic.  It works for the usual 8/16/32/64‑bit unsigned types as
 *  well as for the arbitrary‑precision build.
 */

#include <stdlib.h>

static const seq_t Ncoeff[25] = {
    1, 0,           /* x^0, x^1 */
    276, 0,         /* x^2, x^3 */
    10626, 0,       /* x^4, x^5 */
    136644, 24576,  /* x^6, x^7 */
    870639, 450560, /* x^8, x^9 */
    2975016, 1622016, /* x^10, x^11 */
    4596508,        /* x^12 */
    1622016, 2975016, /* x^13, x^14 */
    450560, 870639, /* x^15, x^16 */
    24576, 136644,  /* x^17, x^18 */
    0, 10626, 0,    /* x^19, x^20, x^21 */
    276, 0,         /* x^22, x^23 */
    1               /* x^24 */
};

static const seq_t binom12[13] = {
    1, 12, 66, 220, 495, 792, 924,
    792, 495, 220, 66, 12, 1
};

/* Compute a(n) for the diamond coordination sequence. */
seq_t a(idx_t n)
{
    /* Allocate array [0..n] of seq_t values.  The size is at most 10000,
       so a simple malloc is acceptable.  The array is freed before return. */
    size_t len = (size_t)n + 1;
    seq_t *buf = (seq_t *)malloc(len * sizeof(seq_t));
    if (!buf) return (seq_t)0;            /* allocation failed */

    /* Main loop.  We build the sequence sequentially, using the
       recurrence and the finite inhomogeneous term Ncoeff. */
    for (idx_t i = 0; i <= n; ++i) {
        seq_t pos = 0, neg = 0;

        /* Add the inhomogeneous contribution N(i) for i <= 24. */
        if (i <= 24)
            pos += Ncoeff[(size_t)i];

        /* Add contributions from the 12 previous terms spaced by 2. */
        for (int k = 1; k <= 12 && (idx_t)(2 * k) <= i; ++k) {
            seq_t term = buf[(size_t)(i - 2 * k)] * binom12[(size_t)k];
            if (k & 1)
                pos += term;   /* (+) coefficient for odd k */
            else
                neg += term;   /* (–) coefficient for even k */
        }

        /* The recurrence gives a(i) = pos - neg.  The result is guaranteed
           to be non‑negative. */
        buf[(size_t)i] = pos - neg;
    }

    seq_t result = buf[(size_t)n];
    free(buf);
    return result;
}
