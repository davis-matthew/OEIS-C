/*
 * A173574  4-Factorions: equal to the sum of the quadruple factorials of
 *          their digits in base 10.
 * https://oeis.org/A173574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, fini, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A173574 – 4‑Factorions: numbers equal to the sum of the
 * quadruple factorials of their decimal digits.  Only six such numbers exist:
 * 1, 2, 3, 4, 5 and 49.  Return 0 for indices beyond the last term. */
seq_t a(idx_t n)
{
    static const seq_t tbl[] = {1, 2, 3, 4, 5, 49};
    if (n == 0) return 0;                                   /* offset starts at 1 */
    if (n <= sizeof(tbl) / sizeof(tbl[0]))
        return tbl[n-1];
    return 0;
}
