/*
 * A080804  Least number of connected subgraphs of the binary cube GF(2)^n
 *          such that every vertex of GF(2)^n lies in at least one of the
 *          subgraphs and no two vertices lie in the same set of subgraphs
 *          (such a collection is called an identifying set).
 * https://oeis.org/A080804
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n + floor(log_2(n))
 * Range:     u8 n <= 248, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n ≥ 1 according to the sequence definition. */
    seq_t result = (seq_t)n;          /* start with n */
    idx_t tmp = n >> 1;                /* shift right by one */
    int log2 = 0;                      /* floor(log2(n)) accumulator */
    while (tmp) {                      /* count how many times we can shift */
        log2++;
        tmp >>= 1;
    }
    result += (seq_t)log2;             /* add the floor of log2(n) */
    return result;
}
