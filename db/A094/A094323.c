/*
 * A094323  a(n) = n*(n+1)*(2*n+1)*(3*n+1)*(4*n+1)/30.
 * https://oeis.org/A094323
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*(n+1)*(2*n+1)*(3*n+1)*(4*n+1)/30
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute using 128‑bit intermediate to avoid overflow for typical limits */
    unsigned __int128 prod = (unsigned __int128)n;
    prod *= (n + 1);
    prod *= (2 * n + 1);
    prod *= (3 * n + 1);
    prod *= (4 * n + 1);
    prod /= 30;                     /* guaranteed integer division */
    return (seq_t)prod;             /* cast back to seq_t */
}
