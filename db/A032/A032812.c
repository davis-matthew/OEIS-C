/*
 * A032812  Numbers whose set of base-12 digits is {2,3}.
 * https://oeis.org/A032812
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the nth positive integer whose base‑12 expansion uses
 *            only the digits 2 and 3. The terms are enumerated increasingly
 * Range:     u8 n <= 6, u16 n <= 46, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Determine the length k (number of base‑12 digits) of a(n). */
    idx_t pow2     = (idx_t)2;   /* 2ⁱ for the current i, starting with i = 1 */
    idx_t cumulative = pow2;    /* Sum of 2¹ … 2ᵏ for current k */
    idx_t k = 1;                /* Current length */

    while (n > cumulative) {
        ++k;                     /* Move to the next length */
        pow2 <<= 1;              /* 2ᵏ  */
        cumulative += pow2;      /* Add 2ᵏ to the running sum */
    }

    /* Offset of n within its length group (0‑based). */
    idx_t m = n - (cumulative - pow2) - 1;   /* m = n - (2ᵏ - 2) - 1 */

    /* Convert the bit pattern m into a base‑12 number with digits 2/3. */
    seq_t val = 0;
    for (idx_t i = 0; i < k; ++i) {
        unsigned bit = (unsigned)((m >> (k - 1 - i)) & 1);
        val = val * (seq_t)12 + (seq_t)(2 + bit);
    }
    return val;
}
