/*
 * A010371  Number of segments used to represent n on a 7-segment calculator
 *          display; version where '6', '7' and '9' use 6, 4 and 6 segments,
 *          respectively.
 * https://oeis.org/A010371
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, nice, look
 * Formula:   a(n) = sum_{d in decimal digits of n} segment_count(d) with
 *            segment_count(0)=6,1=2,2=5,3=5,4=4,5=5,6=6,7=4,8=7,9=6 a(0) = 6
 *            and a(n) = a(floor(n/10)) + a(n mod 10) for n>9
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* segment counts for digits 0..9 */
    static const unsigned char seg[10] = {6, 2, 5, 5, 4, 5, 6, 4, 7, 6};

    if (n == 0)
        return (seq_t)seg[0];

    seq_t sum = 0;
    idx_t tmp = n;
    while (tmp > 0) {
        unsigned int d = (unsigned int)(tmp % 10);
        sum += (seq_t)seg[d];
        tmp /= 10;
    }
    return sum;
}
