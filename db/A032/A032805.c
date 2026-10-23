/*
 * A032805  Numbers whose set of base-5 digits is {2,3}.
 * https://oeis.org/A032805
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   numbers whose base‑5 digits are only 2 or 3
 * Range:     u8 n <= 14, u16 n <= 254, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n ≥ 1 (offset 1) */
    if (n == 0) return 0;          /* defensive, not used by OEIS */

    /* Find the length k of the base‑5 representation.
     * cumulative = Σ_{i=1..k} 2^i = 2^(k+1) – 2
     * pow        = 2^k                            */
    idx_t pow = 2;                 /* 2^1 */
    idx_t cumulative = 2;          /* 2^1 */
    idx_t k = 1;
    while (cumulative < n && pow != 0) {
        pow <<= 1;                 /* next power of two */
        cumulative += pow;         /* add 2^k to total */
        ++k;
    }

    /* offset of the desired number among all k‑digit ones (0‑based) */
    idx_t offset = n - pow + 1;    /* n – 2^k + 1 */

    seq_t val = 0;
    for (idx_t i = 0; i < k; ++i) {
        idx_t bit = (offset >> (unsigned)(k - 1 - i)) & 1;
        val = val * 5 + (bit ? 3 : 2);
    }

    return val;
}
