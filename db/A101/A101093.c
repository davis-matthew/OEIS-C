/*
 * A101093  Second partial sums of sixth powers (A001014).
 * https://oeis.org/A101093
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)^2*(n+2)*(n*(n+2)-1)*(3*n*(n+2)-2)/168
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 25, u64 n <= 422, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0)
        return 0;   /* undefined for n=0, but return 0 just in case */
    unsigned __int128 nn = n;
    unsigned __int128 t1 = nn;                   /* n */
    unsigned __int128 t2 = nn + 1;               /* n+1 */
    unsigned __int128 t3 = nn + 1;               /* n+1 again */
    unsigned __int128 t4 = nn + 2;               /* n+2 */
    unsigned __int128 t5 = nn * (nn + 2) - 1;    /* n*(n+2)-1 */
    unsigned __int128 t6 = 3 * nn * (nn + 2) - 2;/* 3*n*(n+2)-2 */
    unsigned __int128 prod = t1 * t2;
    prod *= t3;
    prod *= t4;
    prod *= t5;
    prod *= t6;
    prod /= 168;
    return (seq_t)prod;
}
