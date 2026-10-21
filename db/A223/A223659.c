/*
 * A223659  Number of unimodal maps [1..n]->[0..3].
 * https://oeis.org/A223659
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = 1 + n*(n+1)*(n^4 + 14*n^3 + 101*n^2 + 304*n + 660)/720
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 118, u64 n <= 4863,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* use a 128‑bit temporary for intermediate arithmetic to avoid overflow */
    unsigned __int128 un = n;                     /* promote n to 128‑bit */

    /* C = n*(n+1)/2  (avoids overflow for large n) */
    unsigned __int128 C = un * (un + 1) / 2;

    /* compute powers of n up to n^4 (all fit in 64‑bit for n ≤ ~1e9) */
    unsigned __int128 n2 = un * un;              /* n^2 */
    unsigned __int128 n3 = n2 * un;              /* n^3 */
    unsigned __int128 n4 = n3 * un;              /* n^4 */

    /* S = n^4 + 14*n^3 + 101*n^2 + 304*n + 660 */
    unsigned __int128 S = n4 + 14 * n3 + 101 * n2 + 304 * un + 660;

    /* numerator = C * S ; denominator = 360 (since 720/2 = 360) */
    unsigned __int128 prod = C * S;
    prod /= 360;

    unsigned __int128 res = prod + 1;            /* final result */

    return (seq_t)res;
}
