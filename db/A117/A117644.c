/*
 * A117644  Number of distinct pairs a < b with nonzero decimal digits such
 *          that a + b = 10^n.
 * https://oeis.org/A117644
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A117644: Number of distinct pairs a < b with non‑zero
   decimal digits such that a + b = 10^n
   Formula: a(1)=4, a(2)=45, a(3)=369; a(n)=9*a(n-1)-8*a(n-2) for n>3 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for n<1, defensive */
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)45;
    if (n == 3) return (seq_t)369;

    seq_t prev2 = (seq_t)45;         /* a(n-2) = a(2)   */
    seq_t prev1 = (seq_t)369;        /* a(n-1) = a(3)   */
    seq_t curr = prev1;              /* placeholder */

    for (idx_t i = 4; i <= n; ++i) {
        curr = (seq_t)9 * prev1 - (seq_t)8 * prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return curr;
}
