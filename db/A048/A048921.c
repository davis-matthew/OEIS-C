/*
 * A048921  9-gonal heptagonal numbers (A000566).
 * https://oeis.org/A048921
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 20163*a(n-1) - 20163*a(n-2) + a(n-3); with a(1) = 1, a(2)
 *            = 26884, a(3) = 542041975
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    if (n == 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)26884;
    if (n == 3)
        return (seq_t)542041975;

    /* Initial values for the recurrence */
    seq_t a1 = (seq_t)1;          /* a(1) */
    seq_t a2 = (seq_t)26884;      /* a(2) */
    seq_t a3 = (seq_t)542041975;  /* a(3) */

    /* Compute a(4) … a(n) iteratively */
    for (idx_t i = 4; i <= n; ++i) {
        seq_t next = (seq_t)20163 * a3 - (seq_t)20163 * a2 + a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}
