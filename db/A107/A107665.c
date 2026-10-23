/*
 * A107665  Numbers with semiprime digits (digits 4, 6, 9 only).
 * https://oeis.org/A107665
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = decimal number composed only of digits 4, 6, 9, obtained
 *            by interpreting the index n‑1 in base‑3 with a length chosen so
 *            that the total count of shorter numbers is < n
 * Range:     u8 n <= 12, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the trivial case (n ≥ 1 is assumed by the caller). */
    if (n == 0) return 0;

    /* Find the length L of the desired number:
       sum_{k=1}^{L-1} 3^k  <  n ≤  sum_{k=1}^{L} 3^k    */
    idx_t cumulative = 0;   /* total count of numbers with length < L */
    idx_t block      = 3;   /* 3^L for the current L (starts with 3^1) */
    idx_t len        = 1;   /* current length L */

    while (cumulative + block < n) {
        cumulative += block;
        block *= 3;
        ++len;
    }

    /* The index within the block of length 'len' numbers (0‑based). */
    idx_t pos = n - (cumulative + 1);

    /* Build the decimal number by extracting base‑3 digits from most to least significant.
       At each step we choose the next digit (0→4, 1→6, 2→9) and append it to the result. */
    seq_t result = 0;
    idx_t factor = block / 3;   /* 3^{len-1} */
    for (idx_t i = 0; i < len; ++i) {
        idx_t digit = pos / factor;    /* 0,1,or 2 */
        pos %= factor;
        result = result * 10 + (digit == 0 ? 4 : digit == 1 ? 6 : 9);
        factor /= 3;
    }
    return result;
}
