/*
 * A037837  a(n) = Sum{|d(i)-d(i-1)|: i=1,...,m}, where Sum{d(i)*5^i:
 *          i=0,1,...,m} is the base 5 representation of n.
 * https://oeis.org/A037837
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Sum{ |d(i)-d(i-1)| : i = 1 … m } where * n = Sum{
 *            d(i)*5^i : i = 0 … m } is the base‑5 representation of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t tmp = n;              /* work on copy of n                                */
    int prev = tmp % 5;         /* least significant base‑5 digit                   */
    tmp /= 5;                   /* shift to next digit                              */
    seq_t sum = 0;              /* accumulation of absolute differences             */
    while (tmp != 0) {
        int cur = tmp % 5;      /* next digit                                    */
        /* abs(cur-prev) – all values are 0…4, cast to seq_t in sum */
        sum += (prev > cur) ? (seq_t)(prev - cur) : (seq_t)(cur - prev);
        prev = cur;            /* advance to next pair                           */
        tmp /= 5;
    }
    return sum;
}
