/*
 * A183679  Number of (n+1) X 7 0..7 arrays with every 2 X 2 subblock summing
 *          to 14.
 * https://oeis.org/A183679
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*8^n + 882*7^n + 11592*6^n + 60690*5^n + 190176*4^n +
 *            420210*3^n + 683592*2^n + 730002
 * Range:     u8 none, u16 none, u32 n <= 6, u64 n <= 18, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t p2 = 1, p3 = 1, p4 = 1, p5 = 1, p6 = 1, p7 = 1, p8 = 1;
    for (idx_t i = 0; i < n; ++i) {
        p2 *= (seq_t)2;
        p3 *= (seq_t)3;
        p4 *= (seq_t)4;
        p5 *= (seq_t)5;
        p6 *= (seq_t)6;
        p7 *= (seq_t)7;
        p8 *= (seq_t)8;
    }
    seq_t res = (seq_t)8 * p8;          /* 8^(n+1)          */
    res += (seq_t)882 * p7;             /* 882*7^n          */
    res += (seq_t)11592 * p6;           /* 11592*6^n        */
    res += (seq_t)60690 * p5;           /* 60690*5^n        */
    res += (seq_t)190176 * p4;          /* 190176*4^n       */
    res += (seq_t)420210 * p3;          /* 420210*3^n       */
    res += (seq_t)683592 * p2;          /* 683592*2^n       */
    res += (seq_t)730002;               /* constant term    */
    return res;
}
