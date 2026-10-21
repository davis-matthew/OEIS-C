/*
 * A083669  Number of ordered quintuples (a,b,c,d,e), -n <= a,b,c,d,e <= n,
 *          such that a+b+c+d+e = 0.
 * https://oeis.org/A083669
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + 5*n*(n+1)*(23*n^2 + 23*n + 14)/12
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 144, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use 128‑bit arithmetic for the intermediate multiplications to avoid overflow,
       then cast back to seq_t. */
    unsigned __int128 N   = (unsigned __int128)n;
    unsigned __int128 part = 5 * N * (N + 1);          /* 5*n*(n+1) */
    unsigned __int128 poly = 23 * N * N + 23 * N + 14; /* 23*n^2 + 23*n + 14 */
    unsigned __int128 prod = part * poly;              /* product */
    prod /= 12;                                       /* divide by 12 */
    unsigned __int128 result = 1 + prod;              /* add the 1 */
    return (seq_t)result;
}
