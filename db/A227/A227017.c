/*
 * A227017  Floor(M(g(n-1)+1,..,g(n))), where M = harmonic mean and g(n) =
 *          n(3n-1)/2 = A000326(n).
 * https://oeis.org/A227017
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 209, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for A227017:
   a(1)=1; a(2)=3; a(3)=8; a(4)=17; a(5)=28;
   a(n)=2*a(n-1)-2*a(n-3)+a(n-4),  for n>=6 */
seq_t a(idx_t n)
{
    if (n <= 5) {
        static const seq_t init[5] = {1, 3, 8, 17, 28};
        return init[n - 1];
    }
    seq_t a1 = 1, a2 = 3, a3 = 8, a4 = 17, a5 = 28;  /* a(1)..a(5) */
    for (idx_t i = 6; i <= n; ++i) {
        seq_t next = 2 * a5 - 2 * a3 + a2;  /* a(n)=2*a(n-1)-2*a(n-3)+a(n-4) */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = next;
    }
    return a5;
}
