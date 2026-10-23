/*
 * A106403  Primitive exponents of the Weyl group W(E_8).
 * https://oeis.org/A106403
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = 2*A005776(n) + 1
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A005776 (exponents of the Weyl group W(E_8))  : 1, 7, 11, 13, 17, 19, 23, 29 */
seq_t a(idx_t n)
{
    /* The sequence has only 8 defined terms. */
    static const seq_t exponents[8] = {
        (seq_t)1,  (seq_t)7,  (seq_t)11, (seq_t)13,
        (seq_t)17, (seq_t)19, (seq_t)23, (seq_t)29
    };
    if (n == 0) return 0;                     /* offset 1; undefined for n < 1 */
    if (n <= (idx_t)sizeof(exponents)/sizeof(exponents[0]))
        return (seq_t)2 * exponents[n - 1] + (seq_t)1;
    return 0;                                 /* undefined beyond the listed terms */
}
