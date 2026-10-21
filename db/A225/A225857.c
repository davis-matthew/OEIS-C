/*
 * A225857  Numbers of the form 2^i*3^j*(12k+1) or 2^i*3^j*(12k+5), i, j, k >=
 *          0.
 * https://oeis.org/A225857
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th smallest positive integer whose 5‑rough * part
 *            is congruent to 1 modulo 4; equivalently, numbers of the form *
 *            2^i * 3^j * (12k+1) or 2^i * 3^j * (12k+5) with i,j,k ≥ 0
 * Range:     u8 n <= 142, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* We count terms until we reach the requested index. */
    idx_t count = 0;
    seq_t num = 1;               /* current integer to test */

    while (count < n)
    {
        seq_t m = num;

        /* Remove all factors of 2. */
        while ((m & 1) == 0)      /* m % 2 == 0  */
            m >>= 1;              /* m /= 2         */

        /* Remove all factors of 3. */
        while (m % 3 == 0)
            m /= 3;

        /* After stripping 2’s and 3’s, the remaining part must be 1 mod 4. */
        if (m % 4 == 1)
        {
            ++count;
            if (count == n)
                return num;
        }
        ++num;
    }

    /* The loop always returns inside; this line is only to silence
       compiler warnings. */
    return 0;
}
