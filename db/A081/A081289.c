/*
 * A081289  a(0) = 0, a(n) = A081293(A081288(n)-1).
 * https://oeis.org/A081289
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0; a(n) = A081293(A081288(n)-1) for n>0, where A081288(n)
 *            = min{i : Catalan(i) > n} and A081293(k) = A000108(k) +
 *            A014137(k) (A014137 = partial sums of the Catalan numbers)
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Validated: partial, pass, 64 terms through n = 63 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nv = (seq_t)n;
    seq_t c = 1;   /* Catalan C(k) */
    seq_t b = 1;   /* sum of C(0..k) */
    idx_t k = 0;

    if (n == 0) return (seq_t)0;

    for (;;) {
        seq_t nc = c * (seq_t)(4 * k + 2) / (seq_t)(k + 2);
        if (nc > nv) break;
        c = nc;
        b += c;
        k++;
    }
    return c + b;
}
