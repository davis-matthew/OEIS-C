/*
 * A221757  Number of 3 X n arrays of occupancy after each element moves to
 *          some horizontal or antidiagonal neighbor, without move-in move-out
 *          straight through or left turns.
 * https://oeis.org/A221757
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 16, u64 n <= 30, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence for A221757:
   a(1)=0, a(2)=4, a(3)=0, a(4)=43, a(5)=44, a(6)=736, a(7)=1680,
   a(8)=14548, a(9)=47314, a(10)=307629.
   For n ≥ 11:
   a(n) = 27*a(n-2) + 14*a(n-3) - 165*a(n-4) - 20*a(n-5)
          + 329*a(n-6) - 48*a(n-7) - 131*a(n-8) - 34*a(n-9)
          + 9*a(n-10);                         */
#include <stddef.h>

static seq_t mul_small(seq_t x, unsigned int m) {
    seq_t res = 0;
    seq_t base = x;
    unsigned int k = m;
    while (k) {
        if (k & 1U) res += base;
        base += base;
        k >>= 1U;
    }
    return res;
}

seq_t a(idx_t n) {
    /* initial values a(1) .. a(10) */
    static const seq_t init[11] = {
        0,            /* unused index 0 */
        0,            /* a(1) */
        4,            /* a(2) */
        0,            /* a(3) */
        43,           /* a(4) */
        44,           /* a(5) */
        736,          /* a(6) */
        1680,         /* a(7) */
        14548,        /* a(8) */
        47314,        /* a(9) */
        307629        /* a(10) */
    };

    if (n <= 10) return init[n];

    seq_t buf[10];
    for (size_t i = 0; i < 10; ++i) buf[i] = init[i + 1];

    seq_t val = 0;
    for (idx_t i = 11; i <= n; ++i) {
        /* positive contributions */
        seq_t pos0 = mul_small(buf[8], 27U);   /* 27*a(n-2) */
        seq_t pos1 = mul_small(buf[7], 14U);   /* 14*a(n-3) */
        seq_t pos2 = mul_small(buf[4], 329U);  /* 329*a(n-6) */
        seq_t pos3 = mul_small(buf[0], 9U);    /* 9*a(n-10) */
        seq_t pos = pos0 + pos1;
        pos += pos2;
        pos += pos3;

        /* negative contributions */
        seq_t neg0 = mul_small(buf[6], 165U);   /* 165*a(n-4) */
        seq_t neg1 = mul_small(buf[5], 20U);    /* 20*a(n-5) */
        seq_t neg2 = mul_small(buf[3], 48U);    /* 48*a(n-7) */
        seq_t neg3 = mul_small(buf[2], 131U);   /* 131*a(n-8) */
        seq_t neg4 = mul_small(buf[1], 34U);    /* 34*a(n-9) */
        seq_t neg = neg0 + neg1;
        neg += neg2;
        neg += neg3;
        neg += neg4;

        seq_t cur = pos - neg;  /* guaranteed non‑negative */

        /* shift buffer left and append new value */
        for (size_t k = 0; k < 9; ++k) buf[k] = buf[k + 1];
        buf[9] = cur;
        val = cur;
    }
    return val;
}
