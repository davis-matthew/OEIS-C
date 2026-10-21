/*
 * A065368  Alternating sum of ternary digits in n. Replace 3^k with (-1)^k in
 *          ternary expansion of n.
 * https://oeis.org/A065368
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, sign
 * Formula:   a(n) = Sum_{k>=0} digit_k(n) * (-1)^k, where digit_k(n) is the
 *            k-th digit of n in base 3 (least significant digit has k = 0)
 * Range:     i8 n <= 103, i16 n <= 103, i32 n <= 103, i64 n <= 103,
 *            i128 n <= 103, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 0;
    seq_t sign = 1;          /* starting with (-1)^0 = +1 */
    idx_t m = n;             /* work on a copy of n */
    while (m > 0) {
        uint64_t digit = m % 3;    /* extract lowest ternary digit */
        result += (seq_t)digit * sign;
        m /= 3;
        sign = -sign;            /* flip sign for next digit */
    }
    return result;
}
