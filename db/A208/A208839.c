/*
 * A208839  Number of nX7 0..1 arrays avoiding 0 0 0 and 0 0 1 horizontally
 *          and 0 1 1 and 1 1 0 vertically.
 * https://oeis.org/A208839
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 11*a(n-1) -17*a(n-2) -124*a(n-3) +276*a(n-4) +396*a(n-5)
 *            -902*a(n-6) -462*a(n-7) +946*a(n-8) +220*a(n-9) -340*a(n-10)
 *            -44*a(n-11) +39*a(n-12) +3*a(n-13) -a(n-14) offset: 1 (a(1)=42)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>   // required by the task, not used here

static const seq_t init_vals[15] = {
    (seq_t)42,                 /* a(1) */
    (seq_t)1764,               /* a(2) */
    (seq_t)7086,               /* a(3) */
    (seq_t)31419,              /* a(4) */
    (seq_t)169974,             /* a(5) */
    (seq_t)992574,             /* a(6) */
    (seq_t)6206445,            /* a(7) */
    (seq_t)40122936,           /* a(8) */
    (seq_t)265568130,          /* a(9) */
    (seq_t)1780946115,         /* a(10) */
    (seq_t)12045557394UL,      /* a(11) */
    (seq_t)81883111842ULL,     /* a(12) */
    (seq_t)558396389421ULL,    /* a(13) */
    (seq_t)3815311552572ULL,   /* a(14) */
    (seq_t)26099962612902ULL  /* a(15) */
};

seq_t a(idx_t n)
{
    if (n <= 15) {
        /* direct look‑up for the initial 15 values */
        return init_vals[n - 1];
    }

    /* buffer holds the 14 most recent values, in order:
       buf[0] = a(k-1), buf[1] = a(k-2), …, buf[13] = a(k-14)   */
    seq_t buf[14];
    for (int i = 0; i < 14; ++i) {
        /* initialise for k = 16: buf[0] = a(15), buf[1] = a(14), …,
           buf[13] = a(2) */
        buf[i] = init_vals[14 - i];
    }

    for (idx_t k = 16; k <= n; ++k) {
        seq_t pos = (seq_t)11 * buf[0]
                  + (seq_t)276 * buf[3]
                  + (seq_t)396 * buf[4]
                  + (seq_t)946 * buf[7]
                  + (seq_t)220 * buf[8]
                  + (seq_t)39  * buf[11]
                  + (seq_t)3   * buf[12];

        seq_t neg = (seq_t)17 * buf[1]
                  + (seq_t)124 * buf[2]
                  + (seq_t)902 * buf[5]
                  + (seq_t)462 * buf[6]
                  + (seq_t)340 * buf[9]
                  + (seq_t)44  * buf[10]
                  + buf[13];

        seq_t next = pos - neg;   /* safe: pos ≥ neg by recurrence definition */

        /* shift buffer: drop oldest value (a(k-14)) and make room for next */
        for (int i = 13; i > 0; --i) {
            buf[i] = buf[i - 1];
        }
        buf[0] = next;   /* new a(k) becomes the most recent value */
    }

    /* after the loop, buf[0] is a(n) */
    return buf[0];
}
