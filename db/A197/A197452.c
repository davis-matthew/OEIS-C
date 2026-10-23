/*
 * A197452  Number of nX3 0..4 arrays with each element x equal to the number
 *          of its horizontal and vertical neighbors equal to 3,3,0,2,2 for
 *          x=0,1,2,3,4.
 * https://oeis.org/A197452
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1)+4*a(n-2)+3*a(n-3)-7*a(n-4)-18*a(n-5)-5*a(n-6)
 *            +18*a(n-7)+31*a(n-8)+20*a(n-9)-5*a(n-10)-25*a(n-11)
 *            -18*a(n-12)-7*a(n-13)+3*a(n-14)+2*a(n-15) (a(1)=2, a(2)=10,
 *            a(3)=31, a(4)=84, a(5)=290, a(6)=923, a(7)=2887, a(8)=9216,
 *            a(9)=29288, a(10)=92868, a(11)=294893, a(12)=936361,
 *            a(13)=2972...
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 38, u128 n <= 76,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mul_small.h"

/* First 15 terms of the sequence */
static const seq_t init[15] = {
    (seq_t)2ULL,      /* a(1) */
    (seq_t)10ULL,     /* a(2) */
    (seq_t)31ULL,     /* a(3) */
    (seq_t)84ULL,     /* a(4) */
    (seq_t)290ULL,    /* a(5) */
    (seq_t)923ULL,    /* a(6) */
    (seq_t)2887ULL,   /* a(7) */
    (seq_t)9216ULL,   /* a(8) */
    (seq_t)29288ULL,  /* a(9) */
    (seq_t)92868ULL,  /* a(10) */
    (seq_t)294893ULL, /* a(11) */
    (seq_t)936361ULL, /* a(12) */
    (seq_t)2972409ULL,/* a(13) */
    (seq_t)9436595ULL,/* a(14) */
    (seq_t)29959267ULL/* a(15) */
};

/* Coefficients of the recurrence, ordered from a(n-1) to a(n-15) */
static const int coeff[15] = {
    2,   /* a(n-1) */
    4,   /* a(n-2) */
    3,   /* a(n-3) */
   -7,   /* a(n-4) */
  -18,   /* a(n-5) */
   -5,   /* a(n-6) */
  18,   /* a(n-7) */
  31,   /* a(n-8) */
  20,   /* a(n-9) */
   -5,   /* a(n-10) */
  -25,   /* a(n-11) */
  -18,   /* a(n-12) */
   -7,   /* a(n-13) */
   3,   /* a(n-14) */
   2    /* a(n-15) */
};

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n <= 15) return init[n - 1];

    /* Store the last 15 terms: prev[0] = a(k-1), prev[1] = a(k-2), … */
    seq_t prev[15];
    /* Initialise with a(15), a(14), …, a(1) */
    for (int i = 0; i < 15; ++i) {
        prev[i] = init[15 - i - 1];
    }

    for (idx_t step = 16; step <= n; ++step) {
        seq_t pos = 0; /* sum of positive coefficient contributions */
        seq_t neg = 0; /* sum of negative coefficient contributions */

        for (int k = 0; k < 15; ++k) {
            if (coeff[k] == 0) continue;
            seq_t term = mul_small(prev[k], (unsigned int)(coeff[k] > 0 ? coeff[k] : -coeff[k]));
            if (coeff[k] > 0)
                pos += term;
            else
                neg += term;
        }

        seq_t next = pos - neg;

        /* Shift the buffer one step */
        for (int j = 14; j > 0; --j)
            prev[j] = prev[j - 1];
        prev[0] = next;
    }

    return prev[0];
}
