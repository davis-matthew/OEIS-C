/*
 * A072479  Surface area of n-dimensional sphere of radius r is n*V_n*r^(n-1)
 *          = n*Pi^(n/2)*r^(n-1)/(n/2)! = S_n*Pi^floor(n/2)*r^(n-1); sequence
 *          gives denominator of S_n.
 * https://oeis.org/A072479
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 14, u32 n <= 22, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The denominator of S_n, the surface‑area coefficient of the n‑dimensional
 * unit sphere. 
 * For n even, a(n)=((n/2-1)!)/2  (with a(0)=a(2)=a(4)=a(6)=1).
 * For n odd, a(n)=(n-2)!!  (double factorial, with a(0)=a(1)=a(3)=1). */
seq_t a(idx_t n)
{
    /* Small indices are trivial */
    if (n <= 4) return (seq_t)1;

    if ((n & 1) == 0) {                /* even n */
        /* n/2 - 1 may be small; handle n <= 6 explicitly */
        idx_t m = n / 2 - 1;
        if (m < 3) return (seq_t)1;   /* n = 6 or less */
        seq_t res = (seq_t)1;
        for (idx_t i = 3; i <= m; ++i) res *= (seq_t)i;
        return res;
    } else {                          /* odd n (n >= 5) */
        idx_t k = n - 2;              /* k is odd */
        seq_t res = (seq_t)1;
        for (idx_t i = 1; i <= k; i += 2) res *= (seq_t)i;
        return res;
    }
}
