/*
 * A107598  Replace even digits d of n with {d/2, d/2}.
 * https://oeis.org/A107598
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, less
 * Formula:   replace each decimal digit d of n, if d is even output two
 *            copies of d/2, otherwise output the digit d itself
 * Range:     u8 n <= 19, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 0;
    /* handle n == 0 implicitly; sequence value will be 0 */
    int digits[64];   /* enough for 64‑bit numbers; max 20 digits + margin */
    int len = 0;
    idx_t tmp = n;
    while (tmp > 0) {
        digits[len++] = tmp % 10;
        tmp /= 10;
    }
    /* if n were 0 len == 0 and res stays 0, matching a(0)=0 */
    for (int i = len - 1; i >= 0; --i) {
        int d = digits[i];
        if ((d & 1) == 0) {          /* even digit */
            int k = d >> 1;          /* d/2 */
            res = res * 10 + k;
            res = res * 10 + k;
        } else {                     /* odd digit stays unchanged */
            res = res * 10 + d;
        }
    }
    return res;
}
