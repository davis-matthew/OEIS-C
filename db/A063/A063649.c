/*
 * A063649  Largest b such that 1/n=1/c+1/b has integer solutions with c>b.
 * https://oeis.org/A063649
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 1381, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A063649 – Largest b such that 1/n = 1/c + 1/b with c > b
 *
 *  Formula: a(n) = n + max{ d | d divides n²  and  d < n }
 *           (or equivalently a(n) = n + the largest proper divisor < n of n²)
 *  Offset: 2
 *
 *  For n < 2 the sequence is undefined; this implementation returns 0.
 *
 *  The implementation simply scans downward from n‑1 to find the
 *  greatest divisor of n² below n.  This is O(n) per call and is
 *  sufficient for the required limits (n up to ≈10 000) while
 *  remaining correct for arbitrary‑precision seq_t.
 */

seq_t a(idx_t n)
{
    /* The sequence is defined only for n >= 2 */
    if (n < 2)
        return (seq_t)0;

    seq_t nsq = (seq_t)n * (seq_t)n;         /* n² as seq_t */

    /* Search for the largest divisor of n² that is < n */
    for (idx_t d = n - 1; d > 0; --d)
    {
        seq_t sd = (seq_t)d;
        if (nsq % sd == 0)
            return (seq_t)n + sd;            /* a(n) = n + d   */
    }

    /* In theory this point is unreachable because 1 always divides n² */
    return (seq_t)n * 2;
}
