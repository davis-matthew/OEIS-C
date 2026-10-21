/*
 * A080310  a(0) = 2 and, for n >= 1, rewrite 0->100 in the binary expansion
 *          of n and append 10 to the right.
 * https://oeis.org/A080310
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(0) = 2; for n ≥ 1, represent n in binary, replace each 1 by
 *            the bit string "1", replace each 0 by the bit string "100", then
 *            append the bit string "10". The resulting binary string is
 *            interpreted as an unsigned integer. This follows the definition
 *            of OEIS A080310
 * Range:     u8 n <= 3, u16 n <= 31, u32 n <= 1023, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)2;   /* special case */

    seq_t res = 0;

    /* find position of most significant bit (0‑based) */
    idx_t tmp = n;
    idx_t msb = 0;
    while (tmp > 0) {
        tmp >>= 1;
        ++msb;
    }
    msb--;            /* msb is now the index of the highest 1 bit */

    for (idx_t i = msb; ; --i) {
        if ( (n >> i) & 1 ) {
            /* bit was 1 → write "1" */
            res = (res << 1) | 1;
        } else {
            /* bit was 0 → write "100" */
            res = (res << 3) | 4;     /* binary 100 = decimal 4 */
        }
        if (i == 0)
            break;
    }

    /* append "10" */
    res = (res << 2) | 2;    /* binary 10 = decimal 2 */

    return res;
}
