/*
 * A043073  a(n)=(s(n)+7)/9, where s(n)=n-th base 9 palindrome that starts
 *          with 2.
 * https://oeis.org/A043073
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (s(n)+7)/9 where s(n) is the nth base‑9 palindrome
 *            starting with 2
 * Range:     u8 n <= 11, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th term of OEIS A043073.
   The indexing is 1‑based: a(1) is the first term. */
seq_t a(idx_t n)
{
    /* 0‑based offset into the sequence of base‑9 palindromes. */
    idx_t offset = n - 1;

    /* Find the digit length L of the palindrome whose order index is offset. */
    idx_t L = 1;
    for (;; ++L) {
        /* half_len = ceil(L/2).  All free digits (except the leading 2)
           are located in positions 1 .. half_len-1. */
        idx_t half_len = (L + 1) / 2;
        idx_t free = half_len - 1;            /* number of free digits */

        /* Count of palindromes of this length: 9^free.  For free==0,
           the count is 1. */
        idx_t count = 1;
        for (idx_t i = 0; i < free; ++i) {
            count *= 9;
        }

        if (offset < count) {
            /* The desired palindrome lies in this length group. */
            break;
        }
        offset -= count;  /* Skip this entire length group. */
    }

    /* half_len and free are now for the chosen length L. */
    idx_t half_len = (L + 1) / 2;
    idx_t free = half_len - 1;

    /* Build the first half of the palindrome (digits[0]..digits[half_len-1]).
       digits[0] must be the leading '2'.  The remaining free digits
       are encoded by the remaining offset in base 9. */
    unsigned char digits[128] = {0};
    digits[0] = 2;

    /* Store the free digits in reverse order (least significant first). */
    unsigned char rev[128];
    for (idx_t i = 0; i < free; ++i) {
        rev[i] = (unsigned char)(offset % 9);
        offset /= 9;
    }

    /* Copy them into the correct order (most significant first). */
    for (idx_t i = 0; i < free; ++i) {
        digits[1 + i] = rev[free - 1 - i];
    }

    /* Mirror the first half onto the second half. */
    for (idx_t i = 0; i < half_len; ++i) {
        digits[L - 1 - i] = digits[i];
    }

    /* Convert the base‑9 digits into a base‑10 number of type seq_t. */
    seq_t val = 0;
    for (idx_t i = 0; i < L; ++i) {
        val = val * 9 + (seq_t)digits[i];
    }

    /* Return a(n) = (s(n)+7)/9. */
    return (val + 7) / 9;
}
