/*
 * A133151  a(n) = smallest k such that A000326(n+1) = A000326(n) +
 *          (A000326(n) mod k), or 0 if no such k exists.
 * https://oeis.org/A133151
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k such that A000326(n+1) = A000326(n) +
 *            (A000326(n) mod k) or 0 if no such k exists. (Offset 1)
 * Range:     u8 n <= 13, u16 n <= 211, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute pentagonal numbers: P(n) = n*(3*n - 1)/2 */
    seq_t nn = (seq_t)n;
    seq_t Pn = nn * (((seq_t)3 * nn) - (seq_t)1) / (seq_t)2;
    seq_t Pnp1 = (nn + (seq_t)1) * (((seq_t)3 * (nn + (seq_t)1)) - (seq_t)1) / (seq_t)2;

    /* Delta = P(n+1) - P(n) */
    seq_t delta = Pnp1 - Pn;

    /* If Pn <= delta, no residue can equal delta (would need k > delta) */
    if (Pn <= delta)
        return (seq_t)0;

    /* X = P(n) - delta; we need a divisor of X that is > delta */
    seq_t X = Pn - delta;

    seq_t best = (seq_t)0; /* 0 means not found yet */

    /* Search divisors up to sqrt(X) */
    for (idx_t i = (idx_t)2; (seq_t)i * (seq_t)i <= X; ++i)
    {
        seq_t si = (seq_t)i;
        if (X % si == (seq_t)0)
        {
            seq_t d1 = si;
            seq_t d2 = X / si;

            if (d1 > delta && (best == (seq_t)0 || d1 < best))
                best = d1;
            if (d2 > delta && (best == (seq_t)0 || d2 < best))
                best = d2;
        }
    }

    /* Consider X itself (if not already handled) */
    if (X > delta && (best == (seq_t)0 || X < best))
        best = X;

    return best;   /* returns 0 if no divisor > delta was found */
}
