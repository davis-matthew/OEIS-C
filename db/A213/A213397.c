/*
 * A213397  Number of (w,x,y) with all terms in {0,...,n} and 2*w >= |x+y-z|.
 * https://oeis.org/A213397
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(2*n^3/3) + 2*n*(n+1) + 1
 * Range:     u8 n <= 6, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for the OEIS sequence A213397.
   Uses 128‑bit arithmetic when the compiler supports it
   to avoid overflow on intermediate calculations. */
seq_t a(idx_t n)
{
    /* Promote to 128‑bit to safely compute 2*n^3/3 */
    unsigned __int128 N = (unsigned __int128)(seq_t)n;

    /* floor(2*n^3/3) */
    unsigned __int128 t1 = (N * N * N * (unsigned __int128)2) / (unsigned __int128)3;

    /* 2*n*(n+1) */
    unsigned __int128 t2 = (unsigned __int128)2 * N * (N + (unsigned __int128)1);

    unsigned __int128 res = t1 + t2 + (unsigned __int128)1;

    return (seq_t)res;   /* safe because the caller expects a value that fits seq_t */
}
