/*
 * A162689  Strictly positive numbers k such that (18*k)/(18+k) are integers.
 * https://oeis.org/A162689
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A162689: strictly positive integers k such that (18*k)/(18+k) is an integer.
   The sequence is finite and consists of the 7 values 9,18,36,63,90,144,306.
   The function determines the nth smallest such k by iterating the divisors d of 324
   that satisfy d ≤ 17 (where d = 18 – j).  The value is k = 324/d – 18. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* out of range */

    idx_t count = 0;
    for (int d = 17; d >= 1; --d)            /* d = 18 – j  (j = 1…17)   */
    {
        if (324 % d == 0)                    /* d must divide 324 (=18²)  */
        {
            ++count;
            if (count == n)
            {
                seq_t val = (seq_t)(324 / d) - 18;
                return val;
            }
        }
    }

    return 0; /* if n > 7 (sequence ends) */
}
