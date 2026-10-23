/*
 * A030377  Position of n-th 3 in A030373.
 * https://oeis.org/A030377
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = position of the n-th occurrence of the digit '3' in the
 *            concatenation of the base‑4 representations of the natural
 *            numbers 1,2,3,... (1‑based positions)
 * Range:     u8 n <= 50, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* If the caller passes 0 we return 0 – the sequence is undefined
       for n = 0, but the value 0 is harmless for any downstream use. */
    if (n == 0) {
        return (seq_t)0;
    }

    seq_t pos = 0;          /* current position in the digit stream */
    idx_t  count = 0;       /* number of 3's seen so far */
    idx_t  num = 1;         /* next natural number to encode */

    /* Temporary buffer to hold the base‑4 digits of the current number.
       32 digits are enough for any 64‑bit integer since
       floor(log4(2^64-1)) = 31. */
    int digits[32];

    while (count < n) {
        /* Decode num into base‑4, storing least‑significant digit first. */
        idx_t tmp = num;
        int len = 0;
        do {
            digits[len++] = (int)(tmp % 4);
            tmp /= 4;
        } while (tmp);

        /* Process the digits from most‑significant to least‑significant,
           updating the position counter and checking for the digit 3. */
        for (int i = len - 1; i >= 0; --i) {
            ++pos;
            if (digits[i] == 3) {
                ++count;
                if (count == n) {
                    return pos;            /* found the n‑th 3 */
                }
            }
        }
        ++num;
    }

    /* The loop always returns before reaching this point. */
    return pos;
}
