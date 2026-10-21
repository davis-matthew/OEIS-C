/*
 * A090003  Length of longest contiguous block of 1's in binary expansion of
 *          n^3.
 * https://oeis.org/A090003
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = longest run of consecutive 1's in binary representation
 *            of n^3
 * Range:     u8 n <= 7, u16 n <= 49, u32 n <= 7985, u64 n <= 7985,
 *            u128 n <= 7985, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t cube = (seq_t)n * (seq_t)n * (seq_t)n;
    seq_t max_run = 0;
    seq_t curr_run = 0;
    while (cube != 0)
    {
        if (cube & 1)
        {
            ++curr_run;
            if (curr_run > max_run)
                max_run = curr_run;
        }
        else
        {
            curr_run = 0;
        }
        cube >>= 1;
    }
    return max_run;
}
