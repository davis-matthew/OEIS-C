/*
 * A212889  Number of (w,x,y,z) with all terms in {0,...,n} and even range.
 * https://oeis.org/A212889
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 17, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A212889: Number of (w,x,y,z) with all terms in {0,…,n} and even range.
 * Recurrence:
 *   a(n) = 2*a(n-1) + 2*a(n-2) - 6*a(n-3) + 6*a(n-5) - 2*a(n-6) - 2*a(n-7) + a(n-8)
 *   for n ≥ 8, with initial values a(0)=1, a(1)=2, a(2)=53, a(3)=104, a(4)=349,
 *   a(5)=594, a(6)=1273, a(7)=1952, a(8)=3401.
 */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Initial terms a(0) … a(8).  They are needed for the recurrence. */
    static const seq_t a_base[9] = {
        (seq_t)1,   /* a(0) */
        (seq_t)2,   /* a(1) */
        (seq_t)53,  /* a(2) */
        (seq_t)104, /* a(3) */
        (seq_t)349, /* a(4) */
        (seq_t)594, /* a(5) */
        (seq_t)1273,/* a(6) */
        (seq_t)1952,/* a(7) */
        (seq_t)3401 /* a(8) */
    };

    /* For n ≤ 8, the value is supplied by the table. */
    if (n < 9)
        return a_base[n];

    /* Buffer holding the last eight computed values:
     *  v[0] = a(n-8), v[1] = a(n-7), …, v[7] = a(n-1).                 */
    seq_t v[8];
    v[0] = a_base[1]; /* a(1)  */
    v[1] = a_base[2]; /* a(2)  */
    v[2] = a_base[3]; /* a(3)  */
    v[3] = a_base[4]; /* a(4)  */
    v[4] = a_base[5]; /* a(5)  */
    v[5] = a_base[6]; /* a(6)  */
    v[6] = a_base[7]; /* a(7)  */
    v[7] = a_base[8]; /* a(8)  */

    seq_t val = 0;
    for (idx_t i = 9; i <= n; ++i) {
        /* Apply the recurrence using the current window of values. */
        val = (seq_t)2 * v[7];          /* 2*a(n-1) */
        val += (seq_t)2 * v[6];         /* +2*a(n-2) */
        val -= (seq_t)6 * v[5];         /* -6*a(n-3) */
        val += (seq_t)6 * v[3];         /* +6*a(n-5) */
        val -= (seq_t)2 * v[2];         /* -2*a(n-6) */
        val -= (seq_t)2 * v[1];         /* -2*a(n-7) */
        val += v[0];                    /* +a(n-8)   */

        /* Slide the window forward: drop a(n-8) and append a(n). */
        for (idx_t j = 0; j < 7; ++j)
            v[j] = v[j + 1];
        v[7] = val;
    }

    return val;
}
