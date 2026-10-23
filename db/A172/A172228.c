/*
 * A172228  Number of ways to place 5 nonattacking wazirs on an n X n board.
 * https://oeis.org/A172228
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            (n^10-50n^8+40n^7+995n^6-1560n^5-8890n^4+21080n^3+24264n^2-97440n+59520)/120
 *            for n ≥ 4, with a(1)=0, a(2)=0, a(3)=1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 15, u64 n <= 136, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the offset and small cases */
    if (n <= 2) return 0;
    if (n == 3) return 1;

    /* n >= 4 */
#ifdef __SIZEOF_INT128__
    /* use 128‑bit intermediate to avoid overflow on small seq_t types */
    __uint128_t N   = n;
    __uint128_t N2  = N * N;          /* n^2 */
    __uint128_t N3  = N2 * N;         /* n^3 */
    __uint128_t N4  = N3 * N;         /* n^4 */
    __uint128_t N5  = N4 * N;         /* n^5 */
    __uint128_t N6  = N5 * N;         /* n^6 */
    __uint128_t N7  = N6 * N;         /* n^7 */
    __uint128_t N8  = N7 * N;         /* n^8 */
    __uint128_t N10 = N8 * N2;        /* n^10 */

    __uint128_t num = N10
                    - 50 * N8
                    + 40 * N7
                    + 995 * N6
                    - 1560 * N5
                    - 8890 * N4
                    + 21080 * N3
                    + 24264 * N2
                    - 97440 * N
                    + 59520;

    return (seq_t)(num / 120);
#else
    /* no 128‑bit support: use seq_t arithmetic (may overflow for very large n,
       but n up to ~125 fits in 64‑bit and is enough for 64‑bit builds) */
    seq_t N   = (seq_t)n;
    seq_t N2  = N * N;          /* n^2 */
    seq_t N3  = N2 * N;         /* n^3 */
    seq_t N4  = N3 * N;         /* n^4 */
    seq_t N5  = N4 * N;         /* n^5 */
    seq_t N6  = N5 * N;         /* n^6 */
    seq_t N7  = N6 * N;         /* n^7 */
    seq_t N8  = N7 * N;         /* n^8 */
    seq_t N10 = N8 * N2;        /* n^10 */

    seq_t num = N10
              - 50 * N8
              + 40 * N7
              + 995 * N6
              - 1560 * N5
              - 8890 * N4
              + 21080 * N3
              + 24264 * N2
              - 97440 * N
              + 59520;

    return num / 120;
#endif
}
