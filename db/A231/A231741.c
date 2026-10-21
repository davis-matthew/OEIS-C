/*
 * A231741  Number of nX2 0..3 arrays with no element less than a strict
 *          majority of its horizontal and vertical neighbors.
 * https://oeis.org/A231741
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10 a(n‑1) – 39 a(n‑2) + 104 a(n‑3) – 251 a(n‑4) + 391
 *            a(n‑5) – 483 a(n‑6) + 687 a(n‑7) – 378 a(n‑8) + 503 a(n‑9) – 247
 *            a(n‑10) – 66 a(n‑11) – 292 a(n‑12) – 300 a(n‑13) – 217 a(n‑14) –
 *            153 a(n‑15) – 70 a(n‑16) – 27 a(n‑17) – 8 a(n‑18) – a(n‑19) for
 *            n > 20, with a(1)=4, a(2)=84, …, a(21)=114...
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Initial terms a(1)...a(21) */
    static const seq_t init[22] = {
        /* 0th dummy */ (seq_t)0,
        (seq_t)4,                  /* a(1) */
        (seq_t)84,                 /* a(2) */
        (seq_t)318,                /* a(3) */
        (seq_t)1328,               /* a(4) */
        (seq_t)6064,               /* a(5) */
        (seq_t)26918,              /* a(6) */
        (seq_t)116909,             /* a(7) */
        (seq_t)511264,             /* a(8) */
        (seq_t)2248196,            /* a(9) */
        (seq_t)9868600,            /* a(10) */
        (seq_t)43259517,           /* a(11) */
        (seq_t)189716276,          /* a(12) */
        (seq_t)832295019,          /* a(13) */
        (seq_t)3650903623,         /* a(14) */
        (seq_t)16013507645,        /* a(15) */
        (seq_t)70240174360,        /* a(16) */
        (seq_t)308101760364,       /* a(17) */
        (seq_t)1351448959786,      /* a(18) */
        (seq_t)5927927566218,      /* a(19) */
        (seq_t)26002016887928,     /* a(20) */
        (seq_t)114054342072628     /* a(21) */
    };

    /* Coefficients for the recurrence (a(n‑1) … a(n‑19)) */
    static const int coeff[19] = {
        10, -39, 104, -251, 391,
        -483, 687, -378, 503, -247,
        -66, -292, -300, -217, -153,
        -70, -27, -8, -1
    };

    /* Return precomputed value if n is within the table */
    if (n <= 21)
        return init[(idx_t)n];

    /* Use a rolling array of the last 19 values.
       last[0] = a(n‑19), last[18] = a(n‑1)  */
    seq_t last[19];
    last[0] = init[3];
    last[1] = init[4];
    last[2] = init[5];
    last[3] = init[6];
    last[4] = init[7];
    last[5] = init[8];
    last[6] = init[9];
    last[7] = init[10];
    last[8] = init[11];
    last[9] = init[12];
    last[10] = init[13];
    last[11] = init[14];
    last[12] = init[15];
    last[13] = init[16];
    last[14] = init[17];
    last[15] = init[18];
    last[16] = init[19];
    last[17] = init[20];
    last[18] = init[21];

    seq_t new_val = 0;
    idx_t i;
    for (idx_t idx = 22; idx <= n; ++idx) {
        new_val = 0;
        for (i = 0; i < 19; ++i)
            new_val += ((seq_t)coeff[i]) * last[18 - i];
        /* Shift the window for the next iteration */
        for (i = 0; i < 18; ++i)
            last[i] = last[i + 1];
        last[18] = new_val;
    }
    return new_val;
}
