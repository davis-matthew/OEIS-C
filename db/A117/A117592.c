/*
 * A117592  a(n) = a(3n) = a(3n+1) = a(3n+2)/2 with a(0)=1.
 * https://oeis.org/A117592
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = product of the nonzero digits of n written in base 3,
 *            which equals 2^(number of digits equal to 2)
 * Range:     u8 n <= 6559, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t tmp = n;
    unsigned int cnt = 0;          /* count of digits equal to 2 */
    while (tmp != 0) {
        if (tmp % 3 == 2) {
            ++cnt;
        }
        tmp /= 3;
    }
    seq_t res = 1;
    while (cnt-- > 0) {
        res *= 2;                  /* multiply by 2 for each 2‑digit */
    }
    return res;
}
