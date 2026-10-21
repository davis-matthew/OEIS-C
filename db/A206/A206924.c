/*
 * A206924  Number of contiguous palindromic bit patterns in the n-th binary
 *          palindrome.
 * https://oeis.org/A206924
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6142, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count contiguous palindromic substrings of the n-th binary palindrome.
   a(1) and a(2) correspond to the palindromes 0 and 1, both of which
   have exactly one palindromic substring.  For n ≥ 3 the function
   finds the n‑th binary palindrome, builds its bit array and then
   enumerates all palindromic substrings by expanding around each
   centre. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* 0 and 1 */
    if (n == 1 || n == 2)
        return (seq_t)1;

    /* Index of the desired palindrome among those of length ≥ 2 */
    idx_t k = n - 2;          /* zero‑based */

    /* Find the length L of the palindrome */
    idx_t cumulative = 0;
    idx_t L = 2;              /* start from length 2 */

    while (1) {
        idx_t half_len = (L + 1) >> 1;          /* ceil(L/2)   */
        idx_t countL   = (idx_t)1 << (half_len - 1); /* 2^(half_len-1) */

        if (cumulative + countL >= k)
            break;
        cumulative += countL;
        ++L;
    }

    /* Offset within palindromes of length L */
    idx_t offset   = k - cumulative - 1;   /* zero based */
    idx_t half_len = (L + 1) >> 1;

    /* Build the bit pattern of the palindrome.
       bits[0] is the most significant bit. */
    int bits[64] = {0};
    bits[0] = 1;           /* leading 1 */

    for (idx_t i = 1; i < half_len; ++i) {
        idx_t pos = half_len - 1 - i;
        bits[i] = (int)((offset >> pos) & 1ULL);
    }

    for (idx_t i = half_len; i < L; ++i) {
        bits[i] = bits[(int)L - 1 - (int)i];
    }

    /* Count all palindromic substrings by centre expansion */
    int L_int = (int)L;
    int cnt   = 0;

    for (int center = 0; center < L_int; ++center) {
        /* Odd length palindromes */
        int l = center, r = center;
        while (l >= 0 && r < L_int && bits[l] == bits[r]) {
            ++cnt;
            --l;
            ++r;
        }
        /* Even length palindromes */
        l = center;
        r = center + 1;
        while (l >= 0 && r < L_int && bits[l] == bits[r]) {
            ++cnt;
            --l;
            ++r;
        }
    }

    return (seq_t)cnt;
}
