/*
 * A048045  Number of nonempty subsets of {1,2,...,n} in which exactly 3/5 of
 *          the elements are <= (n+1)/3.
 * https://oeis.org/A048045
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{t≥1, 5t≤n, 3t≤⌊(n+1)/3⌋, 2t≤n−⌊(n+1)/3⌋}
 *            C(⌊(n+1)/3⌋,3t) · C(n−⌊(n+1)/3⌋,2t)
 * Range:     u8 n <= 13, u16 n <= 24, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Counts the nonempty subsets of {1,…,n} in which exactly 3/5 of the elements are ≤(n+1)/3. */
seq_t a(idx_t n)
{
    /* small set size */
    idx_t A = (n + 1) / 3;
    /* large set size */
    idx_t B = n - A;

    /* maximum t such that 3t ≤ A and 2t ≤ B */
    idx_t tmax = (A / 3 < B / 2) ? (A / 3) : (B / 2);
    if (tmax == 0)
        return (seq_t)0;

    seq_t result = 0;

    /* current binomial values for A and B, starting at C(A,0)=C(B,0)=1 */
    seq_t curA = 1;
    seq_t curB = 1;

    idx_t t;
    for (t = 1; t <= tmax; ++t)
    {
        /* Update C(A, 3t) from C(A, 3(t-1)) in three steps */
        idx_t kA = 3 * (t - 1);
        idx_t i;
        for (i = 1; i <= 3; ++i)
        {
            /* k is current index before multiplication: kA + i - 1 */
            seq_t numer = (seq_t)(A - (kA + i - 1));
            seq_t denom = (seq_t)(kA + i);
            curA = curA * numer / denom;
        }

        /* Update C(B, 2t) from C(B, 2(t-1)) in two steps */
        idx_t kB = 2 * (t - 1);
        for (i = 1; i <= 2; ++i)
        {
            seq_t numer = (seq_t)(B - (kB + i - 1));
            seq_t denom = (seq_t)(kB + i);
            curB = curB * numer / denom;
        }

        result += curA * curB;
    }

    return result;
}
