/*
 * A227560  Number of partitions of n into distinct parts with boundary size
 *          3.
 * https://oeis.org/A227560
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 53, u16 n <= 876, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count partitions of n into distinct parts with boundary size 3. */
seq_t a(idx_t n) {
    if (n < 7) return 0;

    seq_t total = 0;

    /* ----- 1. Three isolated parts (no two consecutive) ----- */
    {
        idx_t max_a = (n - 6) / 3;          /* maximum value of a */
        for (idx_t a = 1; a <= max_a; ++a) {
            idx_t m = n - 6 - 3 * a;        /* remaining amount after fixing a */
            idx_t bmax = m / 2;             /* number of possibilities for the gap offset b1 */
            total += (seq_t)(bmax + 1);
        }
    }

    /* ----- 2. One isolated part and one consecutive block of length >= 2 ----- */
    {
        for (idx_t L = 2; ; ++L) {          /* length of the consecutive block */
            /* Minimal sum of such a block when it starts at 1 */
            idx_t min_sum = (L * (L + 1)) / 2;
            if (min_sum > n - 1)
                break;

            for (idx_t k = 1; ; ++k) {      /* starting integer of the block */
                idx_t sum_block = (L * (2 * k + L - 1)) / 2;
                if (sum_block > n - 1)
                    break;

                idx_t y = n - sum_block;    /* the isolated part */
                if (y >= 1 && (y < k - 1 || y > k + L))
                    ++total;
            }
        }
    }

    return total;
}
