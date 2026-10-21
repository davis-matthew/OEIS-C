/*
 * A080303  Rewrite 0->100 in the binary expansion of n.
 * https://oeis.org/A080303
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = rewrite binary digits of n by replacing every 0 with 100
 * Range:     u8 n <= 7, u16 n <= 63, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 0;
    /* find position of most significant bit of n (msb index) */
    idx_t msb = 0;
    idx_t temp = n;
    while (temp >> 1) {          /* shift until the most significant bit */
        temp >>= 1;
        msb++;
    }
    /* process bits from msb to lsb, building the new binary number */
    for (idx_t i = msb;; --i) {
        if ((n >> i) & 1U) {                  /* original bit is 1 */
            res = (res << 1) | 1;
        } else {                              /* original bit is 0 → append 100 */
            res = (res << 3) | ((seq_t)1 << 2); /* 100b = 4 */
        }
        if (i == 0) break;
    }
    return res;
}
