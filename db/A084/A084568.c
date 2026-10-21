/*
 * A084568  a(0)=1, a(1)=5, a(n+2)=4a(n), n>0.
 * https://oeis.org/A084568
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=5, a(2)=8, a(n)=4*a(n-2) for n>=3
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent expression:
   a(odd n>=3) = 5 * 4^((n-1)/2)
   a(even n>=2) = 2^(n+1) */
/* The implementation computes a(n) using simple loops; it is
   efficient even for n up to thousands. */

seq_t a(idx_t n)
{
    /* Handle the initial terms */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;
    if (n == 2) return (seq_t)8;

    /* For even n >= 2: a(n) = 2^(n+1) */
    if ((n & 1) == 0) {
        seq_t v = (seq_t)1;
        /* Multiply by 2, n+1 times */
        for (idx_t i = 0; i < n + 1; ++i)
            v *= (seq_t)2;
        return v;
    }

    /* For odd n >= 3: a(n) = 5 * 4^((n-1)/2) */
    idx_t m = (n - 1) / 2;          /* number of factors of 4 */
    seq_t v = (seq_t)1;
    for (idx_t i = 0; i < m; ++i)
        v *= (seq_t)4;             /* multiply by 4 each step */
    v *= (seq_t)5;                  /* final factor of 5 */
    return v;
}
