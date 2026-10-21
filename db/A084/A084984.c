/*
 * A084984  Numbers containing no prime digits.
 * https://oeis.org/A084984
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = interpret base‑6 expansion of (n‑1) with digit mapping
 *            c(0)=0, c(1)=1, c(2)=4, c(3)=6, c(4)=8, c(5)=9 for decimal
 *            base‑10 digits
 * Range:     u8 n <= 72, u16 n <= 4536, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t m = n - 1;                      /* work with n‑1 */
    seq_t res  = 0;
    seq_t pow10 = 1;                      /* current power of 10 10^j */

    /* mapping from base‑6 digit d to allowed decimal digit */
    static const unsigned char map[6] = {0, 1, 4, 6, 8, 9};

    /* For n = 1 (m = 0) the loop is skipped and res stays 0,
       which is the first term of the sequence. */

    while (m > 0) {
        unsigned d = m % 6;               /* least‑significant base‑6 digit */
        res += pow10 * map[d];            /* add digit * 10^j */
        pow10 *= 10;                      /* advance to next decimal position */
        m /= 6;
    }
    return res;
}
