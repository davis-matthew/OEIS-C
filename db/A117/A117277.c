/*
 * A117277  Number of partitions of n whose consecutive parts differ by 3.
 * https://oeis.org/A117277
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A117277 – Number of partitions of n into parts that differ by 3.
 *
 * For a partition with k parts we have an arithmetic progression:
 *   a, a+3, a+6, …, a+3(k−1)   (a ≥ 1, k ≥ 1).
 * The sum is n = k*(2a + 3(k−1))/2.
 * The function enumerates all admissible k and checks whether an
 * integer a≥1 exists.  Complexity:  O(√n) per call.
 */

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                 /* offset is 1 – values for n<1 are irrelevant */
    seq_t count = 0;
    for (idx_t k = 1; ; ++k)
    {
        /* Minimum possible sum for this length k: a = 1 */
        seq_t min_sum = ((seq_t)k * ((seq_t)3 * k - 1)) / 2;
        if (min_sum > n)
            break;               /* further k will only increase min_sum */
        if ((seq_t)(2 * n) % k != 0)
            continue;            /* 2n must be divisible by k */
        seq_t t   = (seq_t)(2 * n) / k;
        seq_t val = t - 3 * (k - 1);   /* val = 2a */
        if (val <= 0)
            continue;
        if (val % 2 != 0)
            continue;
        /* a = val/2 is automatically ≥ 1 because val>0 and even */
        ++count;
    }
    return count;
}
