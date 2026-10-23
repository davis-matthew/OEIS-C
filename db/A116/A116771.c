/*
 * A116771  Number of permutations of length n which avoid the patterns 1243,
 *          4132, 4321.
 * https://oeis.org/A116771
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^7 + 21*n^6 - 161*n^5 + 525*n^4 - 1316*n^3 + 9534*n^2 -
 *            31284*n + 37800)/2520 for n≥3 a(1) = 1, a(2) = 2 (Direct
 *            polynomial evaluation, using 128‑bit intermediate to avoid
 *            overflow.)
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    /* compute using 128‑bit integer for safety in the 64‑bit build */
    __int128 nn   = n;          /* cast to 128‑bit */
    __int128 n2   = nn * nn;    /* n^2 */
    __int128 n3   = n2 * nn;    /* n^3 */
    __int128 n4   = n3 * nn;    /* n^4 */
    __int128 n5   = n4 * nn;    /* n^5 */
    __int128 n6   = n5 * nn;    /* n^6 */
    __int128 n7   = n6 * nn;    /* n^7 */

    __int128 numer = n7
                   + (__int128)21 * n6
                   - (__int128)161 * n5
                   + (__int128)525 * n4
                   - (__int128)1316 * n3
                   + (__int128)9534 * n2
                   - (__int128)31284 * nn
                   + (__int128)37800;

    /* 2520 = 7! is a divisor of numer for all integer n */
    return (seq_t)(numer / (__int128)2520);
}
