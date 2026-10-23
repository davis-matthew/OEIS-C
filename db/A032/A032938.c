/*
 * A032938  Numbers whose base-3 representation Sum_{i=0..m} d(i)*3^(m-i) has
 *          d(i)=0 for all odd i.
 * https://oeis.org/A032938
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = numbers whose base‑3 representation has zero digits at
 *            all odd positions (counted from the most significant digit)
 * Range:     u8 n <= 37, u16 n <= 565, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined for n = 0, but return 0 */
    /* Precompute small powers of 3 (sufficient for all practical n).   */
    static idx_t pow3[64];
    static bool pow3_init = false;
    if (!pow3_init)
    {
        pow3[0] = 1;
        for (idx_t i = 1; i < 64; ++i) pow3[i] = pow3[i - 1] * (idx_t)3;
        pow3_init = true;
    }

    /* Find the length L of the base‑3 representation for a(n).   */
    idx_t cumulative = 0;
    idx_t L = 1;
    while (true)
    {
        idx_t count = (idx_t)2 * pow3[(L - 1) / 2];
        if (cumulative + count >= n) break;
        cumulative += count;
        ++L;
    }

    /* Index inside the block of length L (0‑based).                */
    idx_t idx_in_len = n - cumulative - (idx_t)1;

    /* Number of even positions in this length.                     */
    idx_t e_len = (L + (idx_t)1) / (idx_t)2;          /* ceil(L/2) */

    /* Determine the first (most significant) even digit.          */
    idx_t pow_len_minus1 = pow3[e_len - 1];           /* 3^(e_len‑1)  */
    idx_t first_group   = idx_in_len / pow_len_minus1;
    idx_t first_digit   = (idx_t)1 + first_group;     /* 1 or 2 */
    idx_t remainder     = idx_in_len % pow_len_minus1;

    /* Build the vector of even digits (most significant first).   */
    idx_t e_digits[64];
    e_digits[0] = first_digit;
    idx_t rem = remainder;
    for (idx_t pos = 1; pos < e_len; ++pos)
    {
        idx_t power = pow3[e_len - 1 - pos];          /* 3^(e_len‑1‑pos) */
        e_digits[pos] = rem / power;
        rem %= power;
    }

    /* Reconstruct the base‑3 number from the digit pattern.      */
    seq_t val = 0;
    idx_t e_pos = 0;
    for (idx_t i = 0; i < L; ++i)
    {
        seq_t digit = (i & 1) ? 0 : (seq_t)e_digits[e_pos++];
        val = val * (seq_t)3 + digit;
    }

    return val;
}
