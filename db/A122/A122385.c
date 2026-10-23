/*
 * A122385  Smallest m such that (n^2 mod m) = (m^2 mod n).
 * https://oeis.org/A122385
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Computation of OEIS A122385:  smallest m (1 ≤ m ≤ n) such that
   (n² mod m) == (m² mod n).  The function performs a simple linear
   search up to n.  Since n ≤ 10 000 in the tests, this runs easily
   within the required time.
*/
seq_t a(idx_t n)
{
    if (n == 0)                /* undefined at offset 0, return 0 */
        return (seq_t)0;

    /* Scan m from 1 to n.  m = n is always a solution, so the loop
       always terminates. */
    for (idx_t m = 1; m <= n; ++m) {
        /* compute remainders using idx_t arithmetic to avoid overflow
           of the intermediate square. */
        idx_t r1 = ((idx_t)n * (idx_t)n) % m;   /* n² mod m   */
        idx_t r2 = ((idx_t)m * (idx_t)m) % n;   /* m² mod n   */

        if (r1 == r2)
            return (seq_t)m;   /* smallest m found */
    }

    /* Should never be reached – fallback to n. */
    return (seq_t)n;
}
