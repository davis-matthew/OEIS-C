/*
 * A060130  Number of nonzero digits in factorial base representation
 *          (A007623) of n; minimum number of transpositions needed to compose
 *          each permutation in the lists A060117 & A060118.
 * https://oeis.org/A060130
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of nonzero digits in the factorial base
 *            representation of n (A060130)
 * Range:     u8 n <= 40320, u16 n <= 40320, u32 n <= 40320, u64 n <= 40320,
 *            u128 n <= 40320, big any n
 * Validated: standard, pass, 121 terms through n = 9774 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t count = 0;           /* result: count of non‑zero digits */
    idx_t i = 1;               /* position (factorial index) */

    /* Repeatedly extract the next factorial digit.
       digit = n mod (i+1),  n = n div (i+1)   */
    while (n > 0) {
        idx_t digit = n % (i + 1);
        if (digit != 0) ++count;
        n /= (i + 1);
        ++i;
    }

    return count;
}
