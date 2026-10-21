/*
 * A054109  a(n) = T(2*n+1, n), array T as in A054106.
 * https://oeis.org/A054109
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 33, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: (n+1)*a(n) = (3*n+1)*a(n‑1) + 2*(2*n+1)*a(n‑2)
   with a(0)=1, a(1)=2.  This computes a(n)=T(2*n+1,n).  */

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;

    seq_t a_nm2 = (seq_t)1;   /* a(n‑2) */
    seq_t a_nm1 = (seq_t)2;   /* a(n‑1) */
    seq_t a_n   = 0;          /* a(n) */

    for (idx_t i = 2; i <= n; ++i)
    {
        if (sizeof(seq_t) <= 8)          /* 64‑bit or smaller build */
        {
            unsigned __int128 term1 =
                (unsigned __int128)(3*i + 1) * (unsigned __int128)a_nm1;
            unsigned __int128 term2 =
                (unsigned __int128)(2*(2*i + 1)) * (unsigned __int128)a_nm2;
            unsigned __int128 numer = term1 + term2;
            a_n = (seq_t)(numer / (unsigned __int128)(i + 1));
        }
        else                              /* arbitrary‑precision build */
        {
            seq_t term1 = ((seq_t)(3*i + 1)) * a_nm1;
            seq_t term2 = ((seq_t)(2*(2*i + 1))) * a_nm2;
            seq_t numer = term1 + term2;
            a_n = numer / (seq_t)(i + 1);
        }

        /* Shift for next iteration */
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }

    return a_n;
}
