/*
 * A143099  A007318 * A143097.
 * https://oeis.org/A143099
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1, a(2)=3, a(3)=9, a(4)=22, a(5)=50; for n≥6:
 *            a(n)=6*a(n-1)-14*a(n-2)+17*a(n-3)-12*a(n-4)+4*a(n-5)
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 29, u64 n <= 60, u128 n <= 123,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Safety: the sequence is defined for n≥1; the build never calls a(0). */
    if (n == 1) return 1;
    if (n == 2) return 3;
    if (n == 3) return 9;
    if (n == 4) return 22;
    if (n == 5) return 50;

    /* Keep the last five values, a(n‑5)…a(n‑1). */
    seq_t a1 = 1, a2 = 3, a3 = 9, a4 = 22, a5 = 50;
    for (idx_t i = 6; i <= n; ++i) {
        seq_t new_val = 6 * a5 - 14 * a4 + 17 * a3 - 12 * a2 + 4 * a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = new_val;
    }
    return a5;
}
