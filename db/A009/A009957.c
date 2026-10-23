/*
 * A009957  Coordination sequence for FeS2-Pyrite, Fe position.
 * https://oeis.org/A009957
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (13+3*(-1)^n-2*(1+(-1)^n)*n-2*(-18+(-1)^n)*n^2)/8, n>0
 * Range:     u8 n <= 7, u16 n <= 118, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)1;
    /* use 128‑bit intermediate to avoid overflow for large n */
    __int128 nn = (__int128)n * (__int128)n;
    __int128 num;
    if ((n & 1) == 0) {           /* n even */
        num = 34 * nn - 4 * (__int128)n + 16;
    } else {                      /* n odd */
        num = 38 * nn + 10;
    }
    __int128 res = num >> 3;       /* divide by 8 */
    return (seq_t)res;
}
