/*
 * A031453  Numbers whose base-3 representation has one more 0 than 2's.
 * https://oeis.org/A031453
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers whose base-3 representation has one more 0 than 2's.
   Computed by brute‑force search. */
seq_t a(idx_t n)
{
    /* Offset is 1; for n == 0 behaviour is irrelevant, return 0. */
    if (n == 0)
        return (seq_t)0;

    idx_t found = 0;
    /* Search candidate values starting from 1. */
    for (seq_t val = 1; ; ++val)
    {
        seq_t t   = val;
        int  c0   = 0;      /* count of 0's in base‑3 */
        int  c2   = 0;      /* count of 2's in base‑3 */

        /* Count digits in base 3. */
        while (t > 0)
        {
            seq_t d = t % (seq_t)3;
            if (d == 0)
                ++c0;
            else if (d == 2)
                ++c2;
            t /= (seq_t)3;
        }

        /* Check the defining condition: zeros = twos + 1. */
        if (c0 == c2 + 1)
        {
            ++found;
            if (found == n)
                return val;
        }
    }
}
