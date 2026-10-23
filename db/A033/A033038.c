/*
 * A033038  Numbers all of whose base 13 digits are odd.
 * https://oeis.org/A033038
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th positive integer whose base‑13 representation
   consists solely of odd digits (1,3,5,7,9,11). */
seq_t a(idx_t n)
{
    /* Find the length L of the base‑13 number that contains the n‑th term.
       All numbers with L digits have 6^L possibilities (digits 1,3,5,7,9,11). */
    idx_t L = 1;
    idx_t pow6 = 6;        /* 6^L */
    while (n > pow6) {
        n -= pow6;        /* skip all shorter lengths */
        ++L;
        pow6 *= 6;        /* next power of six */
    }

    /* Now n is 1‑based within the current block.  Convert to 0‑based offset. */
    idx_t k = n - 1;

    /* Base‑6 representation of k, padded to L digits. */
    int digits[64] = {0};   /* 64 is plenty for any reasonable n */
    idx_t tmp = k;
    for (int pos = L - 1; pos >= 0; --pos) {
        digits[pos] = tmp % 6;
        tmp /= 6;
    }

    /* Map the base‑6 digits to the allowed odd base‑13 digits. */
    static const int odd[6] = {1, 3, 5, 7, 9, 11};
    seq_t val = 0;
    for (int i = 0; i < L; ++i) {
        val = val * 13 + (seq_t)odd[digits[i]];
    }
    return val;
}
