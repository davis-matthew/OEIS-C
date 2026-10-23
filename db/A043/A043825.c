/*
 * A043825  Numbers k such that number of runs in the base 4 representation of
 *          k is congruent to 0 mod 3.
 * https://oeis.org/A043825
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 56, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers k such that the number of runs in the base‑4 representation of k is congruent to 0 mod 3.
   Computed by enumerating k and checking the run count. */

seq_t a(idx_t n)
{
    /* Offset is 1, so a(1) is the first term.  If n is 0, return 0 (unused). */
    if (n == 0)
        return (seq_t)0;

    idx_t found = 0;                /* how many terms found so far    */
    seq_t k = 0;                    /* current number being tested    */

    while (found < n)
    {
        k++;                        /* candidate k                    */

        /* Count runs in base‑4 representation of k. */
        seq_t tmp = k;
        int prev_digit = -1;        /* previous digit, never equal to 0..3 initially */
        int runs = 0;               /* number of runs seen so far      */

        while (tmp > (seq_t)0)
        {
            int digit = (int)(tmp % (seq_t)4);
            if (digit != prev_digit)
            {
                runs++;
                prev_digit = digit;
            }
            tmp /= (seq_t)4;
        }

        if (runs % 3 == 0)
        {
            found++;
            if (found == n)
                return k;          /* the n-th number matching the property */
        }
    }

    return k; /* unreachable, but keeps compiler happy */
}
