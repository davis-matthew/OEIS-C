/*
 * A228774  Numbers n such that the digits of n, once written in base 16, are
 *          only the hexadecimal digits A to F.
 * https://oeis.org/A228774
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 1553, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A228774: numbers whose hexadecimal digits are only A–F.
   The term a(n) is the (n+1)-st smallest nonnegative integer whose
   hex representation contains only the digits A,B,C,D,E,F.
   The algorithm treats the numbers as base‑6 digits shifted by 10.
   For a given index n, the block length k of the term is chosen such
   that

        sum_{i=1}^{k-1} 6^i  ≤ n < sum_{i=1}^{k} 6^i

   The offset within this block is n - sum_{i=1}^{k-1} 6^i.
   This offset is expressed in base 6 with k digits b_{k-1}…b_0.
   The decimal value of the term is

        Σ_{i=0}^{k-1} (10 + b_i) * 16^i
*/

seq_t a(idx_t n)
{
    /* Find the length k of the block that contains n */
    idx_t sum = 0;       /* cumulative count of terms before current block */
    idx_t pow6 = 6;      /* 6^1 */
    idx_t k = 1;         /* current block length */

    while (sum + pow6 <= n) {
        sum += pow6;    /* finish previous block */
        pow6 *= 6;      /* advance to next block size */
        ++k;
    }

    /* Offset of n within the k‑digit block */
    idx_t offset = n - sum;

    /* Convert offset to base‑6 digits and accumulate result in base‑16 */
    seq_t result = 0;
    seq_t pow16  = 1;    /* 16^0 */

    for (idx_t i = 0; i < k; ++i) {
        idx_t digit = offset % 6;   /* base-6 digit 0..5 */
        offset /= 6;

        seq_t hex_digit = (seq_t)(10 + digit); /* map to A-F -> 10..15 */
        result += hex_digit * pow16;
        pow16 *= 16;
    }

    return result;
}
