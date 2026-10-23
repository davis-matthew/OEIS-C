/*
 * A175917  Convert n to binary. NOR each respective digit of binary n and
 *          binary A030101(n), where A030101(n) is the reversal of the order
 *          of the digits in the binary representation of n (given in
 *          decimal). a(n) is the decimal value of the result.
 * https://oeis.org/A175917
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = for each bit position i (0‑based from LSB) set a(i) = 1
 *            iff bit i of n and bit i of reversed(n) are both 0. Then a(n) is
 *            the integer value of these bits. (a(0)=0 by definition.)
 * Range:     u8 n <= 511, u16 n <= 9806, u32 n <= 9806, u64 n <= 9806,
 *            u128 n <= 9806, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* special case, matches OEIS a(0) */

    /* Determine bit length of n (n > 0 here) */
    idx_t L = 0, tmp = n;
    while (tmp) { ++L; tmp >>= 1; }

    idx_t res = 0;
    for (idx_t j = 0; j < L; ++j) {
        idx_t bit_n  = (n >> j) & 1;
        idx_t bit_rev = (n >> (L - 1 - j)) & 1;   /* bit at symmetric position */
        if (!(bit_n | bit_rev))   /* NOR: only 1 if both bits are 0 */
            res |= ((idx_t)1 << j);
    }
    return (seq_t)res;
}
