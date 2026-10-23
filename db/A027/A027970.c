/*
 * A027970  a(n) = T(n, 2*n-8), T given by A027960.
 * https://oeis.org/A027970
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (-1169280 +1119312*n -517700*n^2 +148092*n^3 -26551*n^4
 *            +2688*n^5 -70*n^6 -12*n^7 +n^8) / 40320 (for n ≥ 4)
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 61, u64 n <= 965,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 4) return 0;            /* behaviour for n < 4 is irrelevant */

    __int128 x = (unsigned __int128)n;   /* use 128‑bit intermediate */
    __int128 val = 1;

    val = val * x - 12;          /* a7 = -12 */
    val = val * x - 70;          /* a6 = -70 */
    val = val * x + 2688;        /* a5 =  2688 */
    val = val * x - 26551;       /* a4 = -26551 */
    val = val * x + 148092;      /* a3 = 148092 */
    val = val * x - 517700;      /* a2 = -517700 */
    val = val * x + 1119312;     /* a1 = 1119312 */
    val = val * x - 1169280;     /* a0 = -1169280 */

    val /= 40320;                /* exact division */

    return (seq_t)val;
}
