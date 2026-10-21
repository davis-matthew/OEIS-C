/*
 * A215090  a(n) = Sum_{i=0..m} d(i)*3^i, where Sum_{i=0..m} d(i)*4^i is the
 *          base-4 representation of n.
 * https://oeis.org/A215090
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = Sum_{i} d_i(n) * 3^i where d_i(n) are the base‑4 digits
 *            of n. Recurrence (derived from the definition): a(n) = 3 *
 *            a(n/4) if n mod 4 == 0 a(n) = a(n-1) + 1 otherwise
 * Range:     u8 n <= 724, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;    /* accumulator for the final value      */
    seq_t power  = 1;    /* 3^i   – grows as we walk through digits  */

    while (n > 0) {
        unsigned digit = n & 3;          /* n % 4   (lowest base‑4 digit)   */
        result += (seq_t)digit * power;  /* add digit*3^i to the running sum */
        power *= 3;                      /* prepare next power of 3         */
        n >>= 2;                         /* n /= 4   (move to next digit)   */
    }

    return result;
}
