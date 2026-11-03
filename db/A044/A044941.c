/*
 * A044941  Number of runs of even length in base-10 representation of n.
 * https://oeis.org/A044941
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base, easy
 * Formula:   a(n) = number of runs of even length in base-10 representation
 *            of n
 * Range:     u8 n <= 65536, u16 n <= 65536, u32 n <= 65536, u64 n <= 65536,
 *            u128 n <= 65536, big any n
 * Validated: standard, pass, 106 terms through n = 7492 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    unsigned prev_digit = 0;
    idx_t run_len = 0;

    while (n > 0)
    {
        unsigned digit = n % 10;
        if (run_len == 0)
        {
            prev_digit = digit;
            run_len = 1;
        }
        else
        {
            if (digit == prev_digit)
            {
                run_len++;
            }
            else
            {
                if ((run_len & 1) == 0)
                    result++;
                prev_digit = digit;
                run_len = 1;
            }
        }
        n /= 10;
    }

    if (run_len > 0 && (run_len & 1) == 0)
        result++;

    return result;
}
