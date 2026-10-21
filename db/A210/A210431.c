/*
 * A210431  Number of semistandard Young tableaux over all partitions of 9
 *          with maximal element <= n.
 * https://oeis.org/A210431
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            n*(10080+(40484+(31395+(8106+655*n^2)*n^2)*n^2)*n^2)/90720
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 20, u64 n <= 239, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    /* Use 128‑bit arithmetic to avoid overflow on intermediate
       values when seq_t is 64‑bit. */
    __int128 nn = n;
    __int128 n2 = nn * nn;                 /* n^2 */
    __int128 t1 = 8106 + 655 * n2;         /* 8106 + 655*n^2 */
    __int128 t2 = t1 * n2;                 /* (8106 + 655*n^2)*n^2 */
    __int128 t3 = 31395 + t2;              /* 31395 + (8106 + 655*n^2)*n^2 */
    __int128 t4 = t3 * n2;                 /* (31395 + …)*n^2 */
    __int128 t5 = 40484 + t4;              /* 40484 + (31395 + …)*n^2 */
    __int128 t6 = t5 * n2;                 /* (40484 + …)*n^2 */
    __int128 t7 = 10080 + t6;              /* 10080 + (40484 + …)*n^2 */
    __int128 num = nn * t7;                /* n * ( ... ) */
    __int128 res = num / 90720;            /* division by 90720 */

    return (seq_t)res;
}
