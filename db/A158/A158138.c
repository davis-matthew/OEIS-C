/*
 * A158138  Number of nondecreasing integer sequences of length 4 with sum
 *          zero and sum of absolute values 2n.
 * https://oeis.org/A158138
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   * a(n) = (⌊n/2⌋+1)^2 + 2*A069905(n), * where A069905(n) =
 *            ⌊(n^2+6n+12)/12⌋ - (⌊n/2⌋+1). * Equivalent closed form: a(n) =
 *            (⌊n/2⌋+1)(⌊n/2⌋-1) + 2*⌊(n^2+6n+12)/12⌋
 * Range:     u8 n <= 23, u16 n <= 395, u32 n <= 999, u64 n <= 999,
 *            u128 n <= 999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Cast n to seq_t for all arithmetic */
    seq_t nseq = (seq_t)n;

    /* t = floor(n/2) + 1 */
    seq_t t = (nseq / 2) + 1;

    /* term1 = t^2 - 2*t = (t)(t-2) */
    seq_t term1 = t * t - 2 * t;

    /* term2 = 2 * floor((n^2+6n+12)/12) */
    seq_t term2 = 2 * ((nseq * nseq + 6 * nseq + 12) / 12);

    return term1 + term2;
}
