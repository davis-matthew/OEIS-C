/*
 * A117796  Enneagonal numbers divisible by 9.
 * https://oeis.org/A117796
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = nth enneagonal number divisible by 9
 * Range:     u8 n <= 2, u16 n <= 22, u32 n <= 5853, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;             /* offset is 1; guard against n==0 */
    idx_t m;
    if ((n & 1) == 1) {               /* n is odd → use n = 9 * ((n-1)/2)   */
        m = 9 * ((n - 1) / 2);
    } else {                           /* n is even → use n = 9 * ((n/2)-1) + 2 */
        m = 9 * ((n / 2) - 1) + 2;
    }
    seq_t num   = (seq_t)m;            /* cast to seq_t for arithmetic */
    seq_t prod  = num * ((((seq_t)7) * num) - 5);
    seq_t value = prod / 2;             /* since product is even */
    return value;
}
