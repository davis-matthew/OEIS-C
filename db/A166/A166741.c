/*
 * A166741  E.g.f.: exp(2*arcsin(x)).
 * https://oeis.org/A166741
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^2 - 4*n + 8) * a(n-2) for n >= 2, with a(0)=1, a(1)=2
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the trivial initial values directly. */
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)2;

    /* For n >= 2, use the parity‑based recurrence. */
    if ((n & 1ULL) == 0) {          /* even n */
        seq_t val = (seq_t)1;       /* a(0) */
        for (idx_t i = 2; i <= n; i += 2) {
            idx_t coef_idx = i * i - 4 * i + 8;
            seq_t coef = (seq_t)coef_idx;
            val = val * coef;
        }
        return val;
    } else {                        /* odd n */
        seq_t val = (seq_t)2;       /* a(1) */
        for (idx_t i = 3; i <= n; i += 2) {
            idx_t coef_idx = i * i - 4 * i + 8;
            seq_t coef = (seq_t)coef_idx;
            val = val * coef;
        }
        return val;
    }
}
