/*
 * A125993  A106486-encodings of combinatorial games with value -1.
 * https://oeis.org/A125993
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 + Σ_{j} 2^(f(j)) , where j runs over the positions of
 *            set bits in (n-1) and f(j) = 3 if j == 0 2*j + 5 otherwise.
 *            (j==0 corresponds to the mandatory bit 1 (value 2); higher bits
 *            encode additional options.)
 * Range:     u8 n <= 4, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)  /* Sequence is not defined for n < 1 */
        return 0;

    seq_t res = (seq_t)2;            /* base: the mandatory bit 1 (value 2) */
    idx_t m   = n - 1;               /* bits of n-1 determine extra options */

    uint32_t pos = 0;                /* position of current bit in m   */
    while (m != 0) {
        if (m & 1) {
            if (pos == 0)
                res += ((seq_t)1) << 3;           /* 2^(1+2*0) = 8  */
            else
                res += ((seq_t)1) << (2*pos + 5); /* 2^(1+2*(2*pos+1)) */
        }
        m >>= 1;
        ++pos;
    }
    return res;
}
