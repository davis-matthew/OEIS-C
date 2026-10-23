/*
 * A175599  The difference between maximal run length and minimal run length
 *          in binary representation of n.
 * https://oeis.org/A175599
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = max_run_length(n) - min_run_length(n) where * max/min run
 *            length are taken over consecutive equal bits in the * binary
 *            representation of n
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* assume n >= 1 */
    // position of most significant bit (0-based)
    int64_t msb = 63 - __builtin_clzll((unsigned long long)n);

    int prev_bit = (int)((n >> msb) & 1);
    int run_len = 1;

    int min_run = 0;
    int max_run = 0;
    int first_run = 1;

    for (int64_t i = msb - 1; i >= 0; --i)
    {
        int bit = (int)((n >> i) & 1);
        if (bit == prev_bit)
        {
            ++run_len;
        }
        else
        {
            /* run ended, update min/max */
            if (first_run)
            {
                min_run = max_run = run_len;
                first_run = 0;
            }
            else
            {
                if (run_len < min_run) min_run = run_len;
                if (run_len > max_run) max_run = run_len;
            }
            /* start new run */
            run_len = 1;
            prev_bit = bit;
        }
    }

    /* finalize last run */
    if (first_run)
    {
        min_run = max_run = run_len;
    }
    else
    {
        if (run_len < min_run) min_run = run_len;
        if (run_len > max_run) max_run = run_len;
    }

    return (seq_t)(max_run - min_run);
}
