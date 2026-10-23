/*
 * A041277  Denominators of continued fraction convergents to sqrt(151).
 * https://oeis.org/A041277
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A041277: Denominators of continued–fraction convergents to √151.
   Recurrence:
      q_{-2} = 1,  q_{-1} = 0
      q_n   = a_n * q_{n-1} + q_{n-2}
   where a_0 = ⌊√151⌋ = 12 and for n≥1 the partial quotients a_n follow
   the periodic sequence of length 20: 3,2,7,1,3,4,1,1,1,11,1,1,1,1,4,3,1,7,2,24. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* 151 and its integer square root 12 */
    const unsigned long long D  = 151ULL;
    const unsigned long long A0 = 12ULL;

    /* Periodic terms of the continued fraction (a1 … a20). */
    static unsigned short *period_terms = NULL;
    static size_t period_len = 0;
    static int initialized = 0;

    if (!initialized) {
        unsigned long long m = 0;
        unsigned long long d = 1;
        unsigned long long a = A0;

        /* Allocate a temporary buffer that will grow as needed */
        size_t cap = 8;
        period_terms = (unsigned short *)malloc(cap * sizeof(unsigned short));
        size_t count = 0;

        do {
            /* Compute next m,d,a */
            m = d * a - m;
            d = (D - m * m) / d;
            a = (A0 + m) / d;

            /* Store the current a as a periodic term (includes the final 24) */
            if (count == cap) {
                cap <<= 1;
                period_terms = (unsigned short *)realloc(period_terms, cap * sizeof(unsigned short));
            }
            period_terms[count++] = (unsigned short)a;
        } while (a != 2 * A0);           /* 2*A0 is the last term of the period */

        period_len   = count;
        initialized  = 1;
    }

    seq_t q_prev2 = (seq_t)1;   /* q_{-2} */
    seq_t q_prev1 = (seq_t)0;   /* q_{-1} */
    seq_t q_curr  = (seq_t)0;

    for (idx_t k = 0; k <= n; ++k) {
        unsigned long long ak;
        if (k == 0) {
            ak = A0;
        } else {
            ak = (unsigned long long)
                 period_terms[(size_t)((k - 1) % period_len)];
        }
        q_curr = (seq_t)ak * q_prev1 + q_prev2;
        q_prev2 = q_prev1;
        q_prev1 = q_curr;
    }
    return q_curr;
}
