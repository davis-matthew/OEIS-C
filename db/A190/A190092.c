/*
 * A190092  Number of rhombuses on a (n+1) X 4 grid.
 * https://oeis.org/A190092
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^2 + 24*n - 56)/2 if n>7 is even (n^2 + 24*n - 57)/2 if
 *            n>7 is odd Base values: a(1)=3, a(2)=10, a(3)=22, a(4)=36,
 *            a(5)=50, a(6)=66, a(7)=82
 * Range:     u8 n <= 9, u16 n <= 200, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Handle base cases directly */
    static const seq_t base[7] = {3, 10, 22, 36, 50, 66, 82};
    if (n <= 7) return base[n - 1];

    /* Compute using the closed‑form expression */
    seq_t N = (seq_t)n;
    seq_t term24 = (seq_t)24 * N;
    seq_t sub = (n & (seq_t)1) ? (seq_t)57 : (seq_t)56; /* odd -> 57, even -> 56 */
    seq_t numer = N * N + term24 - sub;
    return numer / 2;
}
