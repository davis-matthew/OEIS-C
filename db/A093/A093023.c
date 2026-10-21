/*
 * A093023  Indices of terms in A093019 with value 3.
 * https://oeis.org/A093023
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 6553, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A093023: Indices of terms in A093019 with value 3
   A093019(n) returns the check digit required for n to have a valid Luhn
   checksum.  For a number to need the digit 3 we require
   LuhnSum(n) ≡ 7 (mod 10)  because 10−7 = 3.
   Thus a(n) is the n‑th positive integer whose weighted Luhn sum
   modulo 10 equals 7. */

seq_t a(idx_t n)
{
    idx_t count = 0;                      /* how many numbers found so far */
    seq_t val   = 1;                      /* current candidate */

    while (1)
    {
        seq_t sum = 0;
        seq_t tmp = val;
        idx_t pos = 0;                    /* position from the right end */

        /* compute weighted Luhn sum of digits of val */
        while (tmp > 0)
        {
            seq_t digit = tmp % 10;
            if ((pos & 1) == 0)           /* even position (from right) */
            {
                seq_t doubled = digit * 2;
                if (doubled > 9)          /* subtract 9 if doubled > 9 */
                    doubled -= 9;
                sum += doubled;
            }
            else
                sum += digit;

            tmp /= 10;
            pos++;
        }

        if (sum % 10 == 7)               /* required check digit is 3 */
        {
            count++;
            if (count == n)
                return val;
        }

        val++;
    }
}
