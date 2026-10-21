/*
 * A228460  Number of arrays of maxima of three adjacent elements of some
 *          length n+2 0..7 array.
 * https://oeis.org/A228460
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A228460
   Recurrence: a(n) = 8*a(n-1) -28*a(n-2) +56*a(n-3) -42*a(n-4)
                 +168*a(n-6) -48*a(n-7) +195*a(n-8) +340*a(n-9)
                 +154*a(n-10)+408*a(n-11) +432*a(n-12)+276*a(n-13)
                 +338*a(n-14) +268*a(n-15)+149*a(n-16)+118*a(n-17)
                 +71*a(n-18)+28*a(n-19)+14*a(n-20)+6*a(n-21)+a(n-22)
   Initial values (a(1)..a(22)) are listed below. */

#include <stdint.h>

static const int64_t coeff[22] = {
    8LL,  -28LL,   56LL,  -42LL,    0LL,   168LL,  -48LL,
   195LL,  340LL,  154LL,  408LL,   432LL, 276LL, 338LL,
   268LL, 149LL, 118LL,  71LL,    28LL, 14LL,  6LL, 1LL
};

static const seq_t init[22] = {
    8ULL,              /* a(1) */
    64ULL,             /* a(2) */
    372ULL,            /* a(3) */
    1716ULL,           /* a(4) */
    6812ULL,           /* a(5) */
    25096ULL,          /* a(6) */
    92430ULL,          /* a(7) */
    357510ULL,         /* a(8) */
    1453506ULL,        /* a(9) */
    6026658ULL,        /* a(10) */
    24812574ULL,       /* a(11) */
    100499726ULL,      /* a(12) */
    402014614ULL,      /* a(13) */
    1601790692ULL,     /* a(14) */
    6399288759ULL,     /* a(15) */
    25687363699ULL,    /* a(16) */
    103446743321ULL,   /* a(17) */
    416881245069ULL,   /* a(18) */
    1678284756985ULL,  /* a(19) */
    6747301104293ULL,  /* a(20) */
    27104825271091ULL, /* a(21) */
    108875206199331ULL /* a(22) */
};

seq_t a(idx_t n) {
    if (n <= 22) return init[n - 1];

    seq_t state[22];
    /* state[0] = a(n-1), state[1] = a(n-2), … , state[21] = a(n-22) */
    for (int i = 0; i < 22; ++i) state[i] = init[21 - i];

    for (idx_t cur = 23; cur <= n; ++cur) {
        seq_t pos = 0;
        seq_t neg = 0;
        for (int j = 0; j < 22; ++j) {
            int64_t c = coeff[j];
            if (c >= 0) {
                pos += state[j] * (seq_t)c;
            } else {
                neg += state[j] * (seq_t)(-c);
            }
        }
        seq_t val = pos - neg;          /* recurrence result */
        /* shift state and insert new value */
        for (int j = 21; j > 0; --j) state[j] = state[j - 1];
        state[0] = val;
    }
    return state[0];
}
