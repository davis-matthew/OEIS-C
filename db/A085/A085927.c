/*
 * A085927  a(n) is the digitwise absolute difference between the n-th
 *          palindrome and its 9's complement.
 * https://oeis.org/A085927
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = digit-wise |d_i - (9 - d_i)| for the n‑th palindrome
 *            A002113(n)
 * Range:     u8 n <= 18, u16 n <= 198, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function computes the required palindrome and then derives the
   digitwise absolute difference with its 9's complement. */

#include "../utils/pow10.h"

seq_t a(idx_t n) {
    if (n == 0) return 0; /* undefined for n<1, return 0 */

    /* 1. Find the length L of the n‑th palindrome and its index within that group */
    idx_t m = n - 1;               /* 0‑based index */
    idx_t len = 0;                 /* digit length of the palindrome */
    while (1) {
        ++len;
        idx_t half = (len + 1) / 2;
        /* number of palindromes of this length:
           9 * 10^(half-1) */
        idx_t cnt = 9;
        for (idx_t i = 1; i < half; ++i) cnt *= 10;
        if (m < cnt) break;
        m -= cnt;
    }

    /* 2. Compute the first half of the palindrome */
    idx_t half = (len + 1) / 2;
    seq_t first_half = pow10(half - 1) + (seq_t)m;   /* first_half ∈ [10^(half-1), 10^half-1] */

    /* 3. Build the full palindrome p */
    seq_t p = first_half;
    seq_t tmp = first_half;
    if (len % 2 == 1) tmp /= 10;            /* skip the central digit for odd length */
    while (tmp > 0) {
        p = p * 10 + (tmp % 10);
        tmp /= 10;
    }

    /* 4. Compute the digitwise absolute difference with the 9's complement */
    /* The 9's complement of a digit d is (9-d), so the difference is |d-(9-d)| =
       |9-2*d|. The digit d is recovered by iterating over p. */
    seq_t a_val = 0;
    /* collect digits of p in reverse order */
    uint8_t rev[64];    /* sufficient for all practical n; grows with p's length */
    idx_t rev_len = 0;
    seq_t tmp2 = p;
    while (tmp2 > 0) {
        rev[rev_len++] = (uint8_t)(tmp2 % 10);  /* least significant digit */
        tmp2 /= 10;
    }
    /* process digits from most significant to least */
    for (idx_t i = rev_len; i > 0; --i) {
        uint8_t d = rev[i - 1];
        uint8_t diff = (d <= 4) ? 9 - 2 * d : 2 * d - 9;  /* |9-2d| */
        a_val = a_val * 10 + diff;
    }

    return a_val;
}
