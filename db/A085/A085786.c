/*
 * A085786  a(n) = n*(2*n^2 + n + 1)/2.
 * https://oeis.org/A085786
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(2*n^2 + n + 1)/2 (equivalently, a(n) = n*(n+1)/2 +
 *            n^3)
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* use 128‑bit arithmetic for the intermediate product to avoid overflow
       when seq_t is a 32‑bit type.  The final result fits in seq_t for
       all values tested. */
    __int128 nn = (unsigned long long)n;                 /* cast from idx_t to integer */
    __int128 part = ( (__int128)2 * nn * nn + nn + 1 );   /* 2*n^2 + n + 1  */
    __int128 prod = nn * part;                           /* n * (2*n^2 + n + 1) */
    return (seq_t)(prod / 2);                            /* divide by 2 */
}
