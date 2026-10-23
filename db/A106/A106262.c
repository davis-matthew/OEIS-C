/*
 * A106262  An invertible triangle of remainders of 2^n.
 * https://oeis.org/A106262
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = 2^(r‑c) mod (c+2), where r and c are chosen so that the
 *            index n is the (c+1)-st element in the r‑th row of the triangle
 *            T(r,k)=2^(r‑k) mod (k+2); i.e. n = r(r+1)/2 + c, 0 ≤ c ≤ r
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* find row r and column c such that n = r(r+1)/2 + c, 0 ≤ c ≤ r */
    idx_t r = 0;
    idx_t rem = n;
    while (rem >= r + 1) {
        rem -= r + 1;
        r++;
    }
    idx_t c = rem;                /* column index */

    /* exponent = r - c */
    idx_t exp = r - c;

    /* modulo value = c + 2  (at least 2) */
    seq_t mod = (seq_t)(c + 2);

    /* compute 2^exp mod mod by binary exponentiation */
    seq_t base = (seq_t)2 % mod;
    seq_t result = (seq_t)1 % mod;
    while (exp > 0) {
        if (exp & 1) {
            result = (result * base) % mod;
        }
        base = (base * base) % mod;
        exp >>= 1;
    }
    return result;
}
