/*
 * A235527  Number of (n+1)X(1+1) 0..2 arrays with the sum of each 2X2
 *          subblock two median terms lexicographically nondecreasing rowwise
 *          and columnwise.
 * https://oeis.org/A235527
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 12, u64 n <= 28, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A235527:  Number of (n+1)×2 0..2 arrays with the sum of each 2×2 subblock two median terms lexicographically nondecreasing rowwise and columnwise.
   The sequence satisfies the 18‑term linear recurrence
     a(n) = 10*a(n‑1) –10*a(n‑2) –182*a(n‑3) +432*a(n‑4) +1446*a(n‑5) –4430*a(n‑6) –6826*a(n‑7) +23217*a(n‑8) +22320*a(n‑9)
          –70888*a(n‑10) –54080*a(n‑11) +126752*a(n‑12) +91840*a(n‑13) –121152*a(n‑14) –91392*a(n‑15) +46080*a(n‑16) +36864*a(n‑17)
   with initial values
     a(1)=81, a(2)=498, a(3)=2996, a(4)=15782, a(5)=80584,
     a(6)=388008, a(7)=1819537, a(8)=8271237, a(9)=36871545,
     a(10)=161304512, a(11)=695920632, a(12)=2965654390,
     a(13)=12514374020, a(14)=52363323354, a(15)=217577519113,
     a(16)=898716211257, a(17)=3693767937225, a(18)=15117615578422 */

#include <stdint.h>  /* for int64_t */

seq_t a(idx_t n)
{
    /* Initial values for a(1)..a(18) */
    static const seq_t init[18] = {
        81ULL,                 /* a(1)  */
        498ULL,                /* a(2)  */
        2996ULL,               /* a(3)  */
        15782ULL,              /* a(4)  */
        80584ULL,              /* a(5)  */
        388008ULL,             /* a(6)  */
        1819537ULL,            /* a(7)  */
        8271237ULL,            /* a(8)  */
        36871545ULL,           /* a(9)  */
        161304512ULL,          /* a(10) */
        695920632ULL,          /* a(11) */
        2965654390ULL,         /* a(12) */
        12514374020ULL,        /* a(13) */
        52363323354ULL,        /* a(14) */
        217577519113ULL,       /* a(15) */
        898716211257ULL,       /* a(16) */
        3693767937225ULL,      /* a(17) */
        15117615578422ULL      /* a(18) */
    };

    /* Coefficients of the recurrence (signed 64‑bit) */
    static const int64_t coef[18] = {
        10, -10, -182, 432, 1446, -4430, -6826,
        23217, 22320, -70888, -54080, 126752,
        91840, -121152, -91392, 46080, 36864
    };

    if (n <= 18)
        return init[n - 1];

    /* Working buffer for the last 18 terms */
    seq_t last[18];
    for (idx_t i = 0; i < 18; ++i)
        last[i] = init[i];

    for (idx_t step = 19; step <= n; ++step) {
        seq_t sum_pos = 0;
        seq_t sum_neg = 0;

        /* Compute the weighted sum using positive and negative parts */
        for (idx_t i = 0; i < 18; ++i) {
            seq_t term = last[17 - i];          /* a(step-1-i) */
            seq_t mult = term * ((seq_t)(
                              coef[i] >= 0 ? coef[i] : -coef[i]));
            if (coef[i] >= 0)
                sum_pos += mult;
            else
                sum_neg += mult;
        }

        /* The next term is the difference of the two sums */
        seq_t next = sum_pos;
        next -= sum_neg;   /* safe: sum_pos >= sum_neg */

        /* Shift the buffer left and insert the new term */
        for (idx_t i = 0; i < 17; ++i)
            last[i] = last[i + 1];
        last[17] = next;
    }

    return last[17];   /* a(n) */
}
