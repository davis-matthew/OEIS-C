/*
 * A115375  <h[d,d],s[d,d]*s[d,d]*s[d,d]> where h[d,d] is a homogeneous
 *          symmetric function, s[d,d] is a Schur function indexed by two
 *          parts, * represents the Kronecker product and <, > is the standard
 *          scalar product on symmetric functions.
 * https://oeis.org/A115375
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 47, u32 n <= 473, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A115375
    Recurrence:
       a(0)=1, a(1)=1, a(2)=4, a(3)=5, a(4)=12, a(5)=15,
       a(6)=30, a(7)=37, a(8)=65, a(9)=80, a(10)=128, a(11)=156
       a(n)=a(n-1)+4*a(n-2)-3*a(n-3)-7*a(n-4)+2*a(n-5)+8*a(n-6)+2*a(n-7)
             -7*a(n-8)-3*a(n-9)+4*a(n-10)+a(n-11)-a(n-12)   (n>11)
    (a(n) is the standard scalar product <h[d,d],s[d,d]*s[d,d]*s[d,d]>). */

#include <stddef.h>

/* ---------- public function ----------
   Computes the term a(n) of A115375.  The function has
   external linkage and uses the ubiquitous integer types
   supplied by the build system:
      seq_t – unsigned integer or arbitrary‑precision class
      idx_t – unsigned 64‑bit index (uint64_t).          */
seq_t a(idx_t n)
{
    /* Initial 12 terms (by index) */
    static const seq_t init[12] = {
        1,  /* a(0) */
        1,  /* a(1) */
        4,  /* a(2) */
        5,  /* a(3) */
        12, /* a(4) */
        15, /* a(5) */
        30, /* a(6) */
        37, /* a(7) */
        65, /* a(8) */
        80, /* a(9) */
        128,/* a(10) */
        156 /* a(11) */
    };

    /* Return early for the first 12 indices. */
    if (n < 12)
        return init[(size_t)n];

    /* Use a fixed‑size circular buffer of the last 12 values.
       buffer[0] = a[n-12], ..., buffer[11] = a[n-1].  */
    seq_t buf[12];
    for (size_t i = 0; i < 12; ++i)
        buf[i] = init[i];

    /* Iteratively compute subsequent terms up to n. */
    for (idx_t cur = 12; cur <= n; ++cur) {
        /* Separate positive and negative contributions to avoid
           unsigned underflow.  The recurrence guarantees
           that the positive sum >= the negative sum. */
        seq_t pos = buf[11]                     /* a(n-1) */
                  + buf[10] * 4                 /* 4*a(n-2) */
                  + buf[7]  * 2                 /* 2*a(n-5) */
                  + buf[6]  * 8                 /* 8*a(n-6) */
                  + buf[5]  * 2                 /* 2*a(n-7) */
                  + buf[2]  * 4                 /* 4*a(n-10) */
                  + buf[1];                    /*   a(n-11) */

        seq_t neg = buf[9] * 3                 /* 3*a(n-3)  */
                  + buf[8] * 7                 /* 7*a(n-4)  */
                  + buf[4] * 7                 /* 7*a(n-8)  */
                  + buf[3] * 3                 /* 3*a(n-9)  */
                  + buf[0];                    /*   a(n-12) */

        seq_t next = pos - neg;  /* a(n)   */

        /* Shift the buffer left by one position. */
        for (size_t i = 0; i < 11; ++i)
            buf[i] = buf[i + 1];
        buf[11] = next;
    }

    /* After the loop, buf[11] holds a(n). */
    return buf[11];
}
