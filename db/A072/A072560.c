/*
 * A072560  Denominators of w(n) where w(1)=w(2)=w(3)=1,
 *          w(n)=(w(n-1)*w(n-2)+(w(n-1)+w(n-2))/3) / w(n-3).
 * https://oeis.org/A072560
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) equals the denominator of w(n) where w(1)=w(2)=w(3)=1 and
 *            w(n) = (w(n-1)*w(n-2) + (w(n-1)+w(n-2))/3) / w(n-3). The
 *            denominators are periodic with period 18:
 *            (3,9,3,3,1,3,3,9,3,3,1,3,3,9,3,1,1,1). a(1) is the first term
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* pattern of denominators, indexed from 0 to 17 */
    static const seq_t pattern[18] = {
        3, 9, 3, 3, 1, 3, 3, 9,
        3, 3, 1, 3, 3, 9, 3, 1,
        1, 1
    };

    /* indices are 1‑based; subtract 1 to obtain 0‑based index */
    idx_t idx = (n - 1) % 18;
    return pattern[idx];
}
