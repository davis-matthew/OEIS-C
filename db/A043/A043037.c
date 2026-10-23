/*
 * A043037  Base-10 palindromes that start with 2.
 * https://oeis.org/A043037
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th base‑10 palindrome that starts with digit 2
 * Range:     u8 n <= 8, u16 n <= 122, u32 n <= 2222, u64 n <= 2222,
 *            u128 n <= 2222, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* the function is declared elsewhere as seq_t a(idx_t n); */
/* we only provide its definition here. */

seq_t a(idx_t n)
{
    /* Find the length of the palindrome containing the n‑th term. */
    idx_t cumulative = 0;   /* number of terms of shorter lengths */
    idx_t len = 1;          /* current palindrome length */

    while (1) {
        idx_t cnt;          /* number of palindromes of this length */

        if (len == 1 || len == 2) {
            cnt = 1;       /* 2 and 22 are the only ones for lengths 1 and 2 */
        } else {
            idx_t free_digits = (len - 1) / 2;
            cnt = 1;
            for (idx_t i = 0; i < free_digits; ++i)
                cnt *= 10;
        }

        if (cumulative + cnt >= n)
            break;          /* found the target length */

        cumulative += cnt;
        ++len;
    }

    /* Position of the desired palindrome within this length (0‑based). */
    idx_t offset = n - cumulative - 1;

    /* Handle the two trivial cases explicitly. */
    if (len == 1)      return (seq_t)2;
    if (len == 2)      return (seq_t)22;

    /* General case (len ≥ 3). */
    idx_t free_digits = (len - 1) / 2;
    unsigned char free_val[20];  /* digits that are free to choose (most significant side). */

    /* Decompose the offset into decimal digits with leading zeros. */
    idx_t tmp = offset;
    for (idx_t i = free_digits; i > 0; --i) {
        free_val[i - 1] = (unsigned char)(tmp % 10);
        tmp /= 10;
    }

    /* Build the full digit array of the palindrome. */
    unsigned char dig[20] = {0};
    dig[0]      = 2;          /* most significant digit */
    dig[len-1]  = 2;          /* least significant digit */

    for (idx_t i = 0; i < free_digits; ++i) {
        idx_t pos = 1 + i;                 /* position on the left side */
        dig[pos] = free_val[i];
        idx_t mirror = len - 1 - pos;      /* symmetric position on the right side */
        if (mirror != pos)
            dig[mirror] = free_val[i];
    }

    /* Convert the digit array to a numeric value. */
    seq_t val = 0;
    for (idx_t i = 0; i < len; ++i)
        val = val * 10 + dig[i];

    return val;
}
