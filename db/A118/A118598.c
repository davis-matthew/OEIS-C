/*
 * A118598  Palindromes in base 7 (written in base 7).
 * https://oeis.org/A118598
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) are the palindromes written in base 7, interpreted as
 *            decimal numbers (digits 0–6)
 * Range:     u8 n <= 26, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Pre‑compute 7^i up to i=9 (enough for n<=10000). */
    static seq_t pow7[10] = {0};
    static int pow7_init = 0;
    if (!pow7_init) {
        pow7[0] = 1;
        for (int i = 1; i < 10; ++i)
            pow7[i] = pow7[i - 1] * 7;
        pow7_init = 1;
    }

    /* Locate the length L of the palindrome containing a(n). */
    seq_t cumulative = 0;          /* already seen palindromes */
    int L;                          /* number of base‑7 digits */
    for (L = 1; ; ++L) {
        seq_t countLen;
        if (L == 1) {
            countLen = 7;                 /* 0 … 6 */
        } else if (L % 2 == 0) {           /* even length 2k */
            int k = L / 2;
            countLen = 6 * pow7[k - 1];
        } else {                           /* odd length 2k+1 */
            int k = (L - 1) / 2;
            countLen = 6 * pow7[k];
        }
        if (cumulative + countLen >= (seq_t)n)
            break;
        cumulative += countLen;
    }

    /* Index within palindromes of length L (0‑based). */
    seq_t offset_seq = (seq_t)n - cumulative - 1;
    idx_t offset = (idx_t)offset_seq;

    /* Build the digits of the palindrome (base‑7). */
    int digits[20] = {0};            /* L will never exceed 9 */
    int dpos = 0;

    if (L == 1) {                    /* single‑digit palindromes */
        digits[0] = (int)offset;     /* 0 … 6 */
        dpos = 1;
    } else if (L % 2 == 0) {         /* even length 2k */
        int k = L / 2;
        seq_t block = pow7[k - 1];
        int firstIndex = (int)(offset_seq / block);     /* 0 … 5 */
        int firstDigit = 1 + firstIndex;                /* 1 … 6 */
        seq_t rem = offset_seq % block;

        digits[dpos++] = firstDigit;                    /* position 0 */

        /* Remaining k‑1 digits after the leading one */
        for (int pos = 1; pos < k; ++pos) {
            seq_t p = pow7[k - 1 - pos];
            int dig = (int)(rem / p);
            digits[dpos++] = dig;
            rem %= p;
        }

        /* Mirror to complete the palindrome */
        for (int pos = k - 1; pos >= 0; --pos)
            digits[dpos++] = digits[pos];
    } else {                       /* odd length 2k+1 */
        int k = (L - 1) / 2;
        seq_t block = pow7[k];
        int firstIndex = (int)(offset_seq / block);     /* 0 … 5 */
        int firstDigit = 1 + firstIndex;                /* 1 … 6 */
        seq_t rem = offset_seq % block;

        digits[dpos++] = firstDigit;                    /* position 0 */

        /* Next k digits (including the centre) */
        for (int i = 0; i < k; ++i) {
            seq_t p = pow7[k - 1 - i];
            int dig = (int)(rem / p);
            rem %= p;
            digits[dpos++] = dig;
        }

        /* Mirror the first k digits to the right of the centre */
        for (int pos = k - 1; pos >= 0; --pos)
            digits[dpos++] = digits[pos];
    }

    /* Convert the digit array (base‑7) to the decimal value of
       the palindrome string.  Each digit is in the range 0–6,
       so interpreting them as decimal digits gives the required
       sequence value. */
    seq_t result = 0;
    for (int i = 0; i < dpos; ++i)
        result = result * 10 + digits[i];

    return result;
}
