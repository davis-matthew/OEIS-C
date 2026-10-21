/*
 * A203580  a(n) = Sum{d(i)*2^i: i=0,1,...,m}, where Sum{d(i)*7^i:
 *          i=0,1,...,m}=n, d(i)∈{0,1,...,6}.
 * https://oeis.org/A203580
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = interpreting the base‑7 digits of n as a base‑2 number
 *            a(n) = Σ d_i * 2^i where n = Σ d_i * 7^i, 0 ≤ d_i ≤ 6
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t power  = 1;                 /* 2^0 */

    while (n > 0) {
        /* d_i = n mod 7, cast to seq_t to avoid mixing types. */
        seq_t digit = (seq_t)(n % 7);
        result += digit * power;     /* add d_i * 2^i */
        n /= 7;                      /* move to next digit */
        power <<= 1;                 /* 2^(i+1) */
    }

    return result;
}
