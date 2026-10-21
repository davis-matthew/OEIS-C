/*
 * A055662  Successive positions in Tower of Hanoi (with three pegs {0,1,2})
 *          where xyz means smallest disk is on peg z, second smallest is on
 *          peg y, third smallest on peg x, etc. and leading zeros indicate
 *          largest disks are all on peg 0.
 * https://oeis.org/A055662
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=0..floor(log_2(n))} 10^j * (floor((n/2^j + 1)/2) *
 *            (-1)^j mod 3)
 * Range:     u8 n <= 7, u16 n <= 31, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special case for n = 0 */
    if (n == 0) return 0;

    /* Determine the highest j such that 2^j <= n (floor(log2(n))) */
    idx_t t = n;
    int maxj = 0;
    while (t > 0) {
        t >>= 1;
        ++maxj;
    }
    --maxj;  /* zero‑based exponent */

    seq_t result = 0;
    seq_t pow10   = 1;

    for (int j = 0; j <= maxj; ++j) {
        idx_t u = n >> j;                 /* floor(n / 2^j)          */
        idx_t v = (u + 1) >> 1;           /* floor((u + 1) / 2)      */
        int   digit = (int)(v % 3);       /* mod 3 of the above portion */

        if (j & 1)                         /* multiply by (-1)^j mod 3 */
            digit = (digit * 2) % 3;       /* -1 mod 3 is 2 */

        result += pow10 * (seq_t)digit;    /* add digit*10^j to result */
        pow10 *= 10;                       /* prepare next power of ten */
    }

    return result;
}
