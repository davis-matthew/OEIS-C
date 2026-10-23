/*
 * A173009  Expansion of o.g.f. x*(1 - x + x^2)/(1 -3*x +x^2 +3*x^3 -2*x^4).
 * https://oeis.org/A173009
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - a(n-2) - 3*a(n-3) + 2*a(n-4) with initial
 *            values: a(1)=0, a(2)=1, a(3)=2, a(4)=6
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return 0;
    if (n == 2) return 1;
    if (n == 3) return 2;
    if (n == 4) return 6;

    seq_t v1 = 0;  /* a(n-4) */
    seq_t v2 = 1;  /* a(n-3) */
    seq_t v3 = 2;  /* a(n-2) */
    seq_t v4 = 6;  /* a(n-1) */
    idx_t i;

    for (i = 5; i <= n; ++i) {
        seq_t next = 3 * v4 - v3 - 3 * v2 + 2 * v1;
        v1 = v2;
        v2 = v3;
        v3 = v4;
        v4 = next;
    }
    return v4;
}
