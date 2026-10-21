/*
 * A051877  Partial sums of A051740.
 * https://oeis.org/A051877
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*(n+2)*(n+3)*(n+4)*(7*n+5)/120
 * Range:     u8 n <= 3, u16 n <= 14, u32 n <= 146, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A051877. */

/* The function uses 128‑bit arithmetic for intermediate products
   to avoid overflow on typical 64‑bit builds.  The final result
   is cast back to seq_t.  All operations are integer only. */
seq_t a(idx_t n)
{
    /* cast each factor to 128‑bit before multiplication */
    unsigned __int128 prod = 1;
    prod *= (unsigned __int128)(n + 1);
    prod *= (unsigned __int128)(n + 2);
    prod *= (unsigned __int128)(n + 3);
    prod *= (unsigned __int128)(n + 4);
    prod *= (unsigned __int128)(7 * n + 5);
    prod /= 120;                /* 120 = 5 * 24, divides evenly */
    return (seq_t)prod;
}
