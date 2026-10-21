/*
 * A082275  Palindromes k such that k + 11 is also a palindrome.
 * https://oeis.org/A082275
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = d followed by (d-1?) no, specifically: For n ≥ 1 let L =
 *            (n-1)/8 + 2 // length of the palindrome d = (n-1)%8 + 1 // first
 *            and last digit (1 … 8) Then a(n) is the palindrome of length L
 *            whose first and last digit are d and all inner digits are 9.
 *            Example: n=9 → L=3, d=1 → 191. This construc...
 * Range:     u8 n <= 9, u16 n <= 29, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Determine block (zero‑based) and digit d. */
    idx_t block = (n - 1) / 8;                     /* 0‑based block index */
    uint64_t len   = (uint64_t)block + 2;           /* length of the palindrome */
    int d          = (int)((n - 1) % 8) + 1;        /* 1..8 */
    seq_t result   = 0;

    /* Build the palindrome digit by digit. */
    for (uint64_t i = 0; i < len; ++i) {
        seq_t digit = (i == 0 || i == len - 1) ? (seq_t)d : (seq_t)9;
        result = result * 10 + digit;
    }
    return result;
}
