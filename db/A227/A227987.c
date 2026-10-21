/*
 * A227987  If the run lengths of the binary representation of n are [1+r_1,
 *          1+r_2, 1+r_3, ..., 1+r_k], then those of a(n) are [1+(r_1), 1+(r_1
 *          XOR r_2), 1+(r_1 XOR r_2 XOR r_3), ..., 1+(r_1 XOR ... XOR r_k)],
 *          where XOR denotes the XOR binary operator.
 * https://oeis.org/A227987
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is obtained by swapping the bits of n’s run‑length * XOR
 *            transformation described in the OEIS entry. The algorithm *
 *            works directly on the binary representation of n (which fits in
 *            * idx_t) and builds the result in seq_t
 * Range:     u8 n <= 28, u16 n <= 132, u32 n <= 1988, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the (unlikely) case of offset 0. */
    if (n == 0) return 0;

    /* Find the number of bits in n. */
    unsigned bitlen = 0;
    idx_t tmp = n;
    while (tmp) { tmp >>= 1; ++bitlen; }
    if (bitlen == 0) bitlen = 1;          /* for n == 0, not used */

    /* r[i] holds run_length_i - 1, zero‑based indices. */
    unsigned r[128];
    unsigned runs = 0;

    /* Initial run starts with the most significant bit, which is 1. */
    unsigned cur_bit = 1;
    unsigned run_len = 1;

    /* Scan bits from the second most significant down to LSB. */
    if (bitlen > 1) {
        for (int i = (int)bitlen - 2; i >= 0; --i) {
            unsigned bit = (n >> i) & 1u;
            if (bit == cur_bit) {
                ++run_len;
            } else {
                r[runs++] = run_len - 1;
                cur_bit = bit;
                run_len = 1;
            }
        }
    }
    /* Store the last run. */
    r[runs++] = run_len - 1;

    /* Compute prefix XORs to produce y[i] = 1 + (prefix XOR). */
    unsigned pref = 0;
    unsigned y[128];
    for (unsigned i = 0; i < runs; ++i) {
        pref ^= r[i];
        y[i] = 1u + pref;
    }

    /* Build the result from run lengths y[i], alternating bits starting
     * with 1. */
    seq_t res = 0;
    unsigned bit_val = 1;  /* 1 for the first run */
    for (unsigned i = 0; i < runs; ++i) {
        res <<= y[i];
        if (bit_val) {
            res |= ((seq_t)1 << y[i]) - 1;
        }
        bit_val ^= 1;
    }

    return res;
}
