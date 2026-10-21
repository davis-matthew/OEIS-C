/*
 * A093875  Denominators in Kepler's tree of harmonic fractions.
 * https://oeis.org/A093875
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   not stated
 * Range:     u8 n <= 4781, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1.
   For n > 1, a(n) is the denominator of the fraction obtained by
   starting at the root node 1/1 of Kepler's tree and following the
   binary path defined by the bits of n (excluding the leading 1).
   Each step updates the current numerator `num` and denominator `den`
   as follows:
     • left child (bit = 0):   den = den + num   (num unchanged)
     • right child (bit = 1):  num = den_old;  den = num_old + den_old
   The function implements this traversal in O(log n) time. */
seq_t a(idx_t n)
{
    /* Handle the root */
    if (n <= 1)
        return (seq_t)1;

    /* Current numerator and denominator of the node we are at */
    seq_t num = (seq_t)1;
    seq_t den = (seq_t)1;

    /* Find the index of the most significant bit of n (0‑based). */
    idx_t msb = 0;
    for (idx_t t = n; t > 1; t >>= 1)
        msb++;

    /* Process the bits from the next‑most significant down to the
       least significant, skipping the leading 1. */
    for (idx_t i = msb; i-- > 0; ) {
        if ((n >> i) & (idx_t)1) {           /* right child */
            seq_t old_num = num;
            num = den;
            den = old_num + den;
        } else {                             /* left child */
            den += num;
        }
    }

    return den;
}
