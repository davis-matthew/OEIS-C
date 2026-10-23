/*
 * A116955  a(n+1) = a(n) + (if a(n) is odd then (next odd square) else (next
 *          even square)), a(0) = 1.
 * https://oeis.org/A116955
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; for n > 0, a(n) = 2 * (15 + n - 3*n^2 + 2*n^3) / 3
 *            (OEIS entry A116955)
 * Range:     u8 n <= 6, u16 n <= 37, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }

    /* Use a wider native type to avoid overflow for the tested range (n ≤ ~10 000) */
    unsigned long long nn = (unsigned long long)n;
    unsigned long long expr = 15ULL + nn
                              - 3ULL * nn * nn
                              + 2ULL * nn * nn * nn;

    /* expr is always divisible by 3; perform the division before the final multiplication */
    unsigned long long val = (expr / 3ULL) * 2ULL;

    return (seq_t)val;
}
