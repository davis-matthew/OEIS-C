/*
 * A043831  Numbers k such that number of runs in the base 4 representation of
 *          k is congruent to 3 mod 4.
 * https://oeis.org/A043831
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth integer k such that the number of runs in base‑4
 *            representation of k is congruent to 3 (mod 4). Computation is
 *            done by linear search
 * Range:     u8 n <= 56, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t found = 0;        /* number of qualifying integers found so far */
    idx_t k = 1;            /* current integer being tested   */
    seq_t result = 0;       /* the n‑th qualifying integer    */

    while (found < n)
    {
        /* Count runs in the base‑4 representation of k. */
        int runs = 0;
        int prev = -1;
        idx_t temp = k;
        while (temp > 0)
        {
            int digit = (int)(temp & 3);    /* temp % 4, faster with & */
            if (runs == 0 || digit != prev)
            {
                runs++;
                prev = digit;
            }
            temp >>= 2;                      /* divide by 4 */
        }

        if ((runs & 3) == 3)                /* runs % 4 == 3     */
        {
            found++;
            if (found == n)
                result = (seq_t)k;          /* cast to seq_t */
        }
        ++k;
    }

    return result;
}
