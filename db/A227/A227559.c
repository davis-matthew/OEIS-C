/*
 * A227559  Number of partitions of n into distinct parts with boundary size
 *          2.
 * https://oeis.org/A227559
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((n-2)/2) + #{L ≥ 2 | n - L(L-1)/2 is divisible by
 *            L}
 * Range:     u8 n <= 157, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3) return (seq_t)0;          /* behaviour below offset is irrelevant */

    seq_t res = (seq_t)((n - 2) / 2);    /* count unordered non‑consecutive pairs */

    for (idx_t L = 2; ; ++L)
    {
        seq_t min_sum = (seq_t)L * (L + 1) / 2;   /* minimal sum of a chain of length L */
        if (min_sum > n) break;                  /* larger L cannot fit into n */

        seq_t numer = (seq_t)n - (seq_t)L * (L - 1) / 2;
        if (numer % L == 0)                       /* integer k = numer / L exists */
            ++res;                                /* one valid consecutive chain */
    }

    return res;
}
