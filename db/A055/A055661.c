/*
 * A055661  Tower of Hanoi positions (A055662) converted from base 3 to base
 *          10.
 * https://oeis.org/A055661
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = Sum_{j=0..floor(log2(n))} 3^j * ((-1)^j * floor((n/2^j +
 *            1)/2) mod 3)
 * Range:     u8 n <= 31, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* offset 0 */

    seq_t result = 0;
    seq_t pow3   = 1;                     /* 3^j */

    /* maxj = floor(log2(n)) ; n > 0 here */
    uint64_t maxj = 63 - __builtin_clzll(n);

    for (uint64_t j = 0; j <= maxj; ++j) {
        /* t = floor((n/2^j + 1)/2) */
        uint64_t t  = ((n >> j) + 1) >> 1;

        /* only t mod 3 matters */
        uint64_t mod_t = t % 3;
        int     prod   = ((j & 1) ? -(int)mod_t : (int)mod_t);
        int     mod_val = prod % 3;
        if (mod_val < 0) mod_val += 3;    /* 0..2 */

        result += pow3 * (seq_t)mod_val;
        pow3 *= 3;                        /* next power of 3 */
    }

    return result;
}
