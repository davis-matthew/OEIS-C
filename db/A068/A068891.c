/*
 * A068891  a(1) = 1; a(n) = smallest nontrivial n-th power with property that
 *          digits alternate in parity individually as well as in
 *          concatenation with previous terms.
 * https://oeis.org/A068891
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, fini, nonn
 * Formula:   a(n) is the smallest nontrivial n‑th power whose digits
 *            alternate in parity and whose first digit parity differs from
 *            the last digit parity of a(n‑1). If no such number exists, a(n)
 *            = 0
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Known terms of the sequence */
    switch ((uint64_t)n) {
        case 1: return (seq_t)1;
        case 2: return (seq_t)4;
        case 3: return (seq_t)125;
        case 4: return (seq_t)81;
        default: return (seq_t)0;  /* no further terms exist */
    }
}
