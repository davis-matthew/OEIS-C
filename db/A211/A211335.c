/*
 * A211335  Number of (n+1)X(n+1) -5..5 symmetric matrices with every 2X2
 *          subblock having sum zero and one, three or four distinct values.
 * https://oeis.org/A211335
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 13*a(n-1) -40*a(n-2) -130*a(n-3) +771*a(n-4) +305*a(n-5)
 *            -5330*a(n-6) +176*a(n-7) +18744*a(n-8) +2324*a(n-9)
 *            -31120*a(n-10) -14864*a(n-11) +9312*a(n-12) +3072*a(n-13)
 *            -1152*a(n-14) Initial terms: a(1)=47, a(2)=203, a(3)=851,
 *            a(4)=3551, a(5)=14747, a(6)=61279, a(7)=255139, a(8)=1066031,
 *            a(9...
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 28, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Initial 14 terms of the sequence. */
    static const seq_t init[14] = {
        47ULL,          /* a(1) */
        203ULL,         /* a(2) */
        851ULL,         /* a(3) */
        3551ULL,        /* a(4) */
        14747ULL,       /* a(5) */
        61279ULL,       /* a(6) */
        255139ULL,      /* a(7) */
        1066031ULL,     /* a(8) */
        4472467ULL,     /* a(9) */
        18846943ULL,    /* a(10) */
        79770595ULL,    /* a(11) */
        339053711ULL,   /* a(12) */
        1446716915ULL,  /* a(13) */
        6194800095ULL   /* a(14) */
    };

    /* Coefficients of the 14‑term linear recurrence.  Positive values are
       kept directly; negative ones are handled by a subtraction later. */
    static const int64_t coeff[14] = {
        13,      /* a(n-1) */
        -40,     /* a(n-2) */
        -130,    /* a(n-3) */
        771,     /* a(n-4) */
        305,     /* a(n-5) */
        -5330,   /* a(n-6) */
        176,     /* a(n-7) */
        18744,   /* a(n-8) */
        2324,    /* a(n-9) */
        -31120,  /* a(n-10) */
        -14864,  /* a(n-11) */
        9312,    /* a(n-12) */
        3072,    /* a(n-13) */
        -1152    /* a(n-14) */
    };

    if (n == 0) {
        /* Undefined for n < 1 – return 0 as a safe value. */
        return (seq_t)0;
    }

    if (n <= 14) {
        return init[n - 1];
    }

    /* Buffer holding the last 14 terms needed for the recurrence.
       last[0] = a(cur-1), last[1] = a(cur-2), … , last[13] = a(cur-14). */
    seq_t last[14];
    for (idx_t i = 0; i < 14; ++i) {
        last[i] = init[13 - i];  /* a(14), a(13), … , a(1) */
    }

    /* Compute terms up to a(n). */
    for (idx_t cur = 15; cur <= n; ++cur) {
        seq_t sumPos = 0;
        seq_t sumNeg = 0;
        for (int k = 0; k < 14; ++k) {
            seq_t term = last[k];
            int64_t c = coeff[k];
            if (c > 0) {
                sumPos += term * (seq_t)c;
            } else {
                sumNeg += term * (seq_t)(-c);
            }
        }
        seq_t result = sumPos - sumNeg;  /* sumPos > sumNeg by construction. */

        /* Shift the buffer to make room for the new term. */
        for (int i = 13; i > 0; --i) {
            last[i] = last[i - 1];
        }
        last[0] = result;
    }

    return last[0];  /* a(n) */
}
