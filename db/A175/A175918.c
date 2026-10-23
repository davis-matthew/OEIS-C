/*
 * A175918  Convert n to binary. NAND each respective digit of binary n and
 *          binary A030101(n), where A030101(n) is the reversal of the order
 *          of the digits in the binary representation of n (given in
 *          decimal). a(n) is the decimal value of the result.
 * https://oeis.org/A175918
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn, look
 * Formula:   a(n) = binary NAND of n and its binary reversal, interpreted as
 *            a decimal number. a(0) = 0. For n > 0 let L be the number of
 *            bits of n. For i = 0 .. L‑1 b_i = bit (L-1-i) of n, c_i = bit i
 *            of n, r_i = ~(b_i & c_i). The resulting binary string r_0 r_1 …
 *            r_(L‑1) is converted to decimal
 * Range:     u8 n <= 255, u16 n <= 9546, u32 n <= 9546, u64 n <= 9546,
 *            u128 n <= 9546, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0)
        return 0;

    /* determine bit length L of n */
    int L = 0;
    for (idx_t tmp = n; tmp; ++L)
        tmp >>= 1;

    seq_t res = 0;
    for (int i = 0; i < L; ++i) {
        /* bit at high position and corresponding low position */
        idx_t bit_hi = (n >> (L - 1 - i)) & 1;
        idx_t bit_lo = (n >> i) & 1;
        /* NAND bit */
        idx_t nand_bit = 1 ^ (bit_hi & bit_lo);   /* 1 if not both 1 else 0 */
        res = (res << 1) | nand_bit;
    }
    return res;
}
