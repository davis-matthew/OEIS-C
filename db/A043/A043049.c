/*
 * A043049  a(n)=(s(n)+1)/4, where s(n)=n-th base 4 palindrome that starts
 *          with 3.
 * https://oeis.org/A043049
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (s(n)+1)/4, where s(n) is the n-th base‑4 palindrome that
 *            starts with digit 3
 * Range:     u8 n <= 10, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Iterate over palindrome lengths in increasing order.
       Count generated palindromes until the n-th is reached. */
    idx_t count = 0;          /* how many palindromes seen so far */

    for (idx_t len = 1; ; ++len)
    {
        /* half_len = ceil(len/2)   : number of digits that determine the palindrome */
        idx_t half_len = (len + 1) / 2;

        /* number of palindromes of this length (first digit fixed to 3):
           4^(half_len-1).  We compute it using a loop to avoid overflow. */
        idx_t pal_cnt = 1;
        for (idx_t h = 0; h < half_len - 1; ++h)
            pal_cnt *= 4;

        /* Generate all combinations of the remaining half_len-1 digits. */
        for (idx_t i = 0; i < pal_cnt; ++i)
        {
            uint8_t digits[32];   /* enough to hold all half‑digits (len <= 64) */

            digits[0] = 3;           /* first digit fixed to 3 */

            /* Encode i as a base‑4 number of (half_len‑1) digits. */
            idx_t tmp = i;
            for (idx_t pos = half_len - 1; pos >= 1; --pos)
            {
                digits[pos] = (uint8_t)(tmp & 3);
                tmp >>= 2;
            }

            /* Convert the full palindrome to decimal (seq_t). */
            seq_t val = 0;
            for (idx_t k = 0; k < len; ++k)
            {
                seq_t d = (k < half_len)
                           ? ((seq_t)digits[k])
                           : ((seq_t)digits[len - 1 - k]);
                val = val * ((seq_t)4) + d;
            }

            /* Result is (s + 1) / 4. */
            seq_t result = (val + 1) / 4;

            ++count;
            if (count == n)
                return result;
        }
    }
}
