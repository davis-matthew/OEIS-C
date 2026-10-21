/*
 * A082795  Smallest multiple of n beginning with 5.
 * https://oeis.org/A082795
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = the smallest multiple of n whose decimal representation
 *            starts with the digit 5
 * Range:     u8 n <= 11, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the trivial case n == 0, though the sequence is defined only
       for n >= 1.  */
    if (n == 0) return 0;

    seq_t pow10 = 1;   /* Current power of 10: 10^t.  */
    while (1) {
        /* Smallest and largest L‑digit numbers that start with 5:
           5 * 10^t  and  6 * 10^t - 1.  */
        seq_t low  = 5 * pow10;
        seq_t high = 6 * pow10 - 1;

        /* Find the smallest integer k such that k * n >= low. */
        seq_t k = low / n;
        if (low % n) k += 1;

        seq_t m = k * n;  /* Candidate multiple.  */
        if (m <= high)        /* Its leading digit is 5. */
            return m;

        /* No suitable multiple with this many digits; try the next
           length by increasing the power of 10. */
        pow10 *= 10;
    }
}
