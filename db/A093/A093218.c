/*
 * A093218  a(n) is the largest number such that all of a(n)'s length-n
 *          substrings are distinct and divisible by 18.
 * https://oeis.org/A093218
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) builds the integer [ (n-4) 9's ] 8892 [ (n-4) 0's ] 8802 [
 *            (n-4) 0's ] 88 (for n ≥ 4); a(1)=0, a(2)=900, a(3)=99000
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 7, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* trivial cases */
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)900;
    if (n == 3) return (seq_t)99000;

    /* n >= 4 */
    idx_t k = n - 4;                    /* number of 9's and 0's in the blocks   */

    /* compute 10^k in seq_t */
    seq_t pow10k = (seq_t)1;
    for (idx_t i = 0; i < k; ++i)
        pow10k *= (seq_t)10;           /* pow10k = 10^k */

    /* start with the block of k nines: (10^k – 1)   */
    seq_t res = pow10k - (seq_t)1;

    /* append 8892  (4 digits) */
    res = res * (seq_t)10000 + (seq_t)8892;

    /* append k zeros  -> shift by k places  */
    res = res * pow10k;

    /* append 8802  (4 digits) */
    res = res * (seq_t)10000 + (seq_t)8802;

    /* append k zeros again   */
    res = res * pow10k;

    /* append the final 88 (2 digits) */
    res = res * (seq_t)100 + (seq_t)88;

    return res;
}
