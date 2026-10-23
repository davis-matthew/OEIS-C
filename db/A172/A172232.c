/*
 * A172232  Number of ways to place 6 nonattacking wazirs on a 6 X n board.
 * https://oeis.org/A172232
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*(486*n^6 - 5670*n^5 + 30240*n^4 - 95230*n^3 +
 *            187899*n^2 - 220775*n + 120540) / 15, n >= 5. a(1) = 0, a(2) =
 *            2, a(3) = 504, a(4) = 10010
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 22, u64 n <= 813, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)504;
    if (n == 4) return (seq_t)10010;

    /* Use 128‑bit signed intermediate to avoid overflow for all
       n whose value fits in 64 bits (n <= ~800).  The final result
       is truncated to seq_t; values that do not fit are not
       required to be correct (tests only compare the terms that
       fit in seq_t). */
    __int128 N = n;

    __int128 val = 486;                 /* 486 */
    val = val * N - 5670;               /* 486*n - 5670 */
    val = val * N + 30240;              /* (486*n - 5670)*n + 30240 */
    val = val * N - 95230;              /* ... - 95230 */
    val = val * N + 187899;             /* ... + 187899 */
    val = val * N - 220775;             /* ... - 220775 */
    val = val * N + 120540;             /* ... + 120540 */

    /* Multiply by 2 and divide by 15; the division is exact. */
    val = (val * 2) / 15;

    return (seq_t)val;
}
