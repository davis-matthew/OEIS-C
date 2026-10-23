/*
 * A147759  Palindromes formed from the reflected decimal expansion of the
 *          infinite concatenation of 1's and 0's.
 * https://oeis.org/A147759
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = palindrome of length n formed by taking the first *
 *            ceil(n/2) digits of the infinite alternating‑1‑0 string *
 *            (1,0,1,0,…) and mirroring them to generate a palindrome
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For safety, handle n == 0, although OEIS offset starts at 1. */
    if (n == 0)
        return (seq_t)0;

    /* k = ceil(n/2) = (n+1)/2 in integer arithmetic */
    idx_t k = (n + 1) / 2;
    seq_t res = 0;

    for (idx_t i = 1; i <= n; ++i) {
        /* Position in the alternating string that determines the digit.
         * For i <= k we use i directly, otherwise use mirror position. */
        idx_t pos = (i <= k) ? i : (n - i + 1);

        /* Digit is 1 if pos is odd, 0 otherwise. */
        seq_t digit = (pos & (idx_t)1) ? (seq_t)1 : (seq_t)0;

        res = res * (seq_t)10 + digit;
    }
    return res;
}
