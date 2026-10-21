/*
 * A201238  Number of ways to place 4 non-attacking wazirs on an n X n
 *          toroidal board.
 * https://oeis.org/A201238
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 23, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula   : a(n) = n^2*(n^2-11)*(n^4-19*n^2+114)/24   for n >= 5
 * Special  : a(1)=a(2)=a(3)=0 ,  a(4)=228 .
 * All operations are performed in a 128‑bit intermediate to avoid overflow
 * on 64‑bit builds; the final result is cast to seq_t.
 */
seq_t a(idx_t n)
{
    if (n < 5)
    {
        if (n == 4)
            return (seq_t)228;   /* a(4) */
        else
            return (seq_t)0;     /* a(1), a(2), a(3) */
    }

    __int128 nn   = n;            /* promote to 128‑bit */
    __int128 n2   = nn * nn;      /* n^2            */
    __int128 n4   = n2 * n2;      /* n^4            */
    __int128 t2   = n2 - 11;      /* n^2 - 11       */
    __int128 t3   = n4 - 19 * n2 + 114; /* n^4 - 19n^2 + 114 */

    __int128 prod = n2 * t2;      /* intermediate   */
    prod *= t3;                   /* full product   */
    prod /= 24;                   /* divide by 24   */

    return (seq_t)prod;           /* cast down to seq_t */
}
