/*
 * A039059  Numbers whose base-8 representation has the same number of 1's and
 *          5's.
 * https://oeis.org/A039059
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS A039059 (Numbers whose base‑8
 * representation has the same number of 1's and 5's) by brute‑force search. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n < 1, but return 0 as safe value */

    seq_t candidate = 0;  /* current number being tested */
    idx_t found = 0;      /* how many qualifying numbers have been found */

    while (found < n)
    {
        int c1 = 0, c5 = 0;
        seq_t temp = candidate;

        /* Count 1’s and 5’s in the base‑8 representation of candidate */
        while (temp > 0)
        {
            seq_t d = temp % (seq_t)8;
            if (d == (seq_t)1) c1++;
            else if (d == (seq_t)5) c5++;
            temp = temp / (seq_t)8;
        }

        if (c1 == c5)
        {
            ++found;
            if (found == n) return candidate;
        }

        ++candidate;
    }
    return 0; /* unreachable, keeps compiler happy */
}
