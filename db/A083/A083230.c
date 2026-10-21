/*
 * A083230  Number of repunit divisors of n.
 * https://oeis.org/A083230
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 943, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A083230 – Number of repunit divisors of n
 *  Formula:  a(n) = |{ R(k) : R(k) | n }|  where R(k) = (10^k‑1)/9
 */

seq_t a(idx_t n)
{
    /* n is guaranteed to be ≥ 1, but be tolerant of 0 if called so */
    if (n == 0)
        return (seq_t)0;

    seq_t N = (seq_t)n;            /* value to test */
    seq_t r = (seq_t)1;            /* current repunit R(k) */
    seq_t count = (seq_t)0;

    /* threshold: the largest r such that the next repunit r*10+1
     * will still be ≤ N.  (N‑1)/10 takes care of the final step.   */
    seq_t threshold = (N - (seq_t)1) / (seq_t)10;

    while (1) {
        if (N % r == (seq_t)0)     /* r divides N ? */
            count += (seq_t)1;

        if (r > threshold)         /* next repunit would exceed N */
            break;

        r = r * (seq_t)10 + (seq_t)1;  /* R(k+1) = R(k)*10 + 1 */
    }

    return count;
}
