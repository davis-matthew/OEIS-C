/*
 * A043614  Numbers whose base-6 representation has exactly 6 runs.
 * https://oeis.org/A043614
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A043614: numbers whose base‑6 representation has exactly
    6 runs.  This implementation iterates through positive integers
    until the n‑th integer with 6 runs is found.  Runs are counted by
    a helper that scans the digits of a number in base 6. */

static int count_runs_base6(seq_t x)
{
    int runs = 0;
    int prev = -1;
    while (x > 0)
    {
        int digit = (int)(x % 6);
        if (digit != prev)
        {
            ++runs;
            prev = digit;
        }
        x /= 6;
    }
    return runs;
}

seq_t a(idx_t n)
{
    if (n == 0)   /* undefined for n == 0; return 0 by convention */
        return 0;

    idx_t found = 0;
    seq_t val = 0;

    for (;; ++val)
    {
        if (count_runs_base6(val) == 6)
        {
            ++found;
            if (found == n)
                return val;
        }
    }
}
