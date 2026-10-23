/*
 * A153028  Special values of the hypergeometric function of the type 4F0.
 * https://oeis.org/A153028
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 11, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A153028 - Special values of the hypergeometric function of the type 4F0.
   Recurrence:
      4*a(n) - 4*a(n-1) - 4*(n-1)*(n-2)*(n-3)*a(n-2)
          + (n-5)*(n-1)*(n-2)*(n-4)*(n-3)^2*a(n-4) = 0
   Initial values: a(0)=a(1)=a(2)=a(3)=1
*/
#if defined(__SIZEOF_INT128__)
typedef unsigned __int128 u128;
#endif

seq_t a(idx_t n)
{
    /* For n < 4 the value is 1. */
    if (n < 4)
        return (seq_t)1;

    /* Sliding window: a(n-4), a(n-3), a(n-2), a(n-1) */
    seq_t a0 = (seq_t)1; /* a(0) */
    seq_t a1 = (seq_t)1; /* a(1) */
    seq_t a2 = (seq_t)1; /* a(2) */
    seq_t a3 = (seq_t)1; /* a(3) */

    for (idx_t i = 4; i <= n; ++i)
    {
        seq_t term1 = a3; /* a(i-1) */
        seq_t term2 = a2; /* a(i-2) */
        seq_t term3 = a0; /* a(i-4) */

#if defined(__SIZEOF_INT128__)
        /* 128‑bit intermediate to avoid overflow when seq_t is a small
           built‑in type.  All computations are performed modulo 2^128
           but division by 4 is exact for the recurrence. */
        u128 P = (u128)(i - 1) * (u128)(i - 2) * (u128)(i - 3);          /* (i-1)(i-2)(i-3)   */
        u128 Q = 0;                                                      /* (i-5)(i-1)(i-2)(i-4)(i-3)^2 */
        if (i >= 5) {
            Q = (u128)(i - 5) * (u128)(i - 1) * (u128)(i - 2) *
                (u128)(i - 4);
            u128 tmp = (u128)(i - 3);
            tmp *= tmp;            /* (i-3)^2 */
            Q *= tmp;
        }

        u128 num = (u128)4 * (u128)term1;               /* 4*a(i-1) */
        num += (u128)4 * P * (u128)term2;              /* 4*P*a(i-2) */
        if (i >= 5)
            num -= Q * (u128)term3;                    /* -Q*a(i-4) */

        seq_t next = (seq_t)(num / 4);                  /* a(i) = (num)/4 */
#else
        /* Without 128‑bit support use seq_t arithmetic directly.
           The coefficient (n-5)*(...) is always an integer, so the
           division by 4 is performed after the summation. */
        seq_t P = (seq_t)(i - 1);
        P *= (seq_t)(i - 2);
        P *= (seq_t)(i - 3);                            /* (i-1)(i-2)(i-3)   */

        seq_t Q = 0;                                     /* (i-5)(i-1)(i-2)(i-4)(i-3)^2 */
        if (i >= 5) {
            Q = (seq_t)(i - 5);
            Q *= (seq_t)(i - 1);
            Q *= (seq_t)(i - 2);
            Q *= (seq_t)(i - 4);
            seq_t tmp = (seq_t)(i - 3);
            tmp *= tmp;                                 /* (i-3)^2 */
            Q *= tmp;
        }

        seq_t num = (seq_t)4 * term1;                   /* 4*a(i-1) */
        num += (seq_t)4 * P * term2;                    /* 4*P*a(i-2) */
        if (i >= 5)
            num -= Q * term3;                           /* -Q*a(i-4) */

        seq_t next = num / (seq_t)4;                    /* a(i) = (num)/4 */
#endif
        /* Shift the sliding window for the next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }

    return a3; /* a(n) */
}
