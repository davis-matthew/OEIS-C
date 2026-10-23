/*
 * A037455  a(n)=Sum{d(i)*7^i: i=0,1,...,m}, where Sum{d(i)*3^i: i=0,1,...,m}
 *          is the base 3 representation of n.
 * https://oeis.org/A037455
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 728, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A037455: a(n) = Sum_{i>=0} d_i(n) * 7^i, where d_i(n) are the base‑3 digits of n.
   In other words, interpret the base‑3 representation of n as a base‑7 number. */
seq_t a(idx_t n) {
    idx_t m = n;
    seq_t result = 0;
    seq_t power  = 1;              /* 7^0 */
    while (m > 0) {
        unsigned digit = m % 3;    /* base‑3 digit */
        result += power * digit;
        power *= 7;                /* next power of 7 */
        m /= 3;
    }
    return result;
}
