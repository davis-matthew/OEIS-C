/*
 * A193760  Replace 3^i with n^i in ternary representation of n.
 * https://oeis.org/A193760
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sum of the ternary digits of n weighted by powers of n
 *            i.e. interpret the base‑3 representation of n as a base‑n number
 * Range:     u8 n <= 15, u16 n <= 39, u32 n <= 215, u64 n <= 1457,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;
    seq_t base = (seq_t)n;           /* base for the powers of n   */
    seq_t res   = 0;                  /* running sum                */
    seq_t power = 1;                  /* n^0                       */
    idx_t m = n;                      /* current value to process   */
    while (m > 0) {
        idx_t digit = m % 3;          /* next ternary digit         */
        if (digit != 0) {
            res += (seq_t)digit * power;
        }
        power = power * base;         /* n^(i+1) for next digit    */
        m = m / 3;                    /* move to next ternary digit */
    }
    return res;
}
