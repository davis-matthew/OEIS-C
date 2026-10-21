/*
 * A081241  Position in B of reversal of n-th term of B, where B is the
 *          logic-binary sequence, A007931.
 * https://oeis.org/A081241
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, look
 * Formula:   a(n) = value of the reversed bijective base‑2 representation of
 *            n
 * Range:     u8 n <= 255, u16 n <= 9102, u32 n <= 9102, u64 n <= 9102,
 *            u128 n <= 9102, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;               /* below offset – undefined requirement */

    /* digits of the bijective base‑2 representation (least significant first) */
    unsigned char digits[65];          /* enough for idx_t up to 64 bits */
    int len = 0;
    idx_t t = n;

    while (t > 0) {
        unsigned char d = (unsigned char)(((t - 1) & 1ULL) + 1); // digit 1 or 2
        digits[len++] = d;
        t = (t - 1) >> 1;
    }

    /* the digits are already in reverse order, so compute the number directly */
    seq_t res = 0;
    for (int i = 0; i < len; ++i)
        res = res * 2 + digits[i];

    return res;
}
