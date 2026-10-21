/*
 * A208216  Number of ways of making change for n cents using coins of 1, 5,
 *          10, and 25 cents, where each coin that appears is used a different
 *          number of times.
 * https://oeis.org/A208216
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ways to make change for n cents using the coin
 *            denominations 1, 5, 10, and 25 such that any two coin types that
 *            are used have different positive multiplicities
 * Range:     u8 n <= 114, u16 n <= 784, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The trivial case: zero cents can be made in exactly one way
       (by using no coins). */
    if (n == 0)
        return (seq_t)1;

    seq_t count = 0;

    /* Enumerate all possible numbers of 25‑cent coins. */
    for (idx_t d = 0; d <= n / 25; ++d)
    {
        idx_t rem_after_d = n - 25 * d;          /* amount left after 25¢ coins */

        /* Enumerate all possible numbers of 10‑cent coins. */
        for (idx_t c = 0; c <= rem_after_d / 10; ++c)
        {
            idx_t rem_after_c = rem_after_d - 10 * c;  /* remaining amount */

            /* Enumerate all possible numbers of 5‑cent coins. */
            for (idx_t b = 0; b <= rem_after_c / 5; ++b)
            {
                /* The number of 1‑cent coins is forced by the remaining amount. */
                idx_t a_val = rem_after_c - 5 * b;   /* a_val >= 0 */

                /* Distinctness checks: if two used coin types have the same
                   multiplicity (positive), discard this combination. */
                if (d > 0)
                {
                    if (c > 0 && d == c) continue;
                    if (b > 0 && d == b) continue;
                    if (a_val > 0 && d == a_val) continue;
                }
                if (c > 0)
                {
                    if (b > 0 && c == b) continue;
                    if (a_val > 0 && c == a_val) continue;
                }
                if (b > 0)
                {
                    if (a_val > 0 && b == a_val) continue;
                }

                /* All constraints satisfied; count this way of making change. */
                count++;
            }
        }
    }

    return count;
}
