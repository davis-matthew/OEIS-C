/*
 * A089273  Fifth column (k=6) of array A078739(n,k) ((2,2)-generalized
 *          Stirling2).
 * https://oeis.org/A089273
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 70*a(n-1) - 1708*a(n-2) + 17544*a(n-3) - 72000*a(n-4) +
 *            86400*a(n-5) for n >= 5 Initial values (OEIS offset 0): a(0) =
 *            1, a(1) = 188, a(2) = 12052, a(3) = 540080, a(4) = 20447056
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)188;
    if (n == 2) return (seq_t)12052;
    if (n == 3) return (seq_t)540080;
    if (n == 4) return (seq_t)20447056;

    seq_t a0 = (seq_t)1;
    seq_t a1 = (seq_t)188;
    seq_t a2 = (seq_t)12052;
    seq_t a3 = (seq_t)540080;
    seq_t a4 = (seq_t)20447056;

    for (idx_t i = 5; i <= n; ++i) {
        seq_t next = 70 * a4 - 1708 * a3 + 17544 * a2 - 72000 * a1 + 86400 * a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = next;
    }
    return a4;
}
