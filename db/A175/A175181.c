/*
 * A175181  Pisano period of the 2-Fibonacci numbers A000129.
 * https://oeis.org/A175181
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the Pisano period of the Pell (2‐Fibonacci) numbers
 *            modulo n (sequence A000129). The Pell numbers satisfy P_0 = 0,
 *            P_1 = 1, P_k = 2·P_{k-1} + P_{k-2} (k≥2). The period is the
 *            smallest positive t such that (P_t, P_{t+1}) ≡ (0,1) (mod n),
 *            i.e. the first return of the pair (0,1) in the recurr...
 * Range:     u8 n <= 120, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the trivial case n = 1 */
    if (n == 1)
        return (seq_t)1;

    seq_t prev = 0;
    seq_t curr = 1 % (seq_t)n;
    seq_t target1 = 1 % (seq_t)n;   /* target pair (0, target1) */
    idx_t  step   = 0;

    do
    {
        ++step;
        /* compute next = (2*curr + prev) mod n using a larger type to avoid overflow */
        unsigned long long t = ((unsigned long long)curr * 2ULL +
                               (unsigned long long)prev) % (unsigned long long)n;
        prev = curr;
        curr = (seq_t)t;
    } while (!(prev == 0 && curr == target1));

    return (seq_t)step;
}
