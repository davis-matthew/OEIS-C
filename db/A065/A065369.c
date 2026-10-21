/*
 * A065369  Replace 3^k with (-3)^k in ternary expansion of n.
 * https://oeis.org/A065369
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, look, sign
 * Formula:   a(n) = Σ_{k≥0} d_k · (-3)^k where d_k are the base‑3 digits of n
 * Range:     i8 n <= 161, i16 n <= 9843, i32 n <= 9843, i64 n <= 9843,
 *            i128 n <= 9843, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 0;        /* result */
    seq_t p   = 1;        /* (-3)^k, starting with k = 0 */
    idx_t x = n;

    while (x > 0) {
        unsigned d = x % 3;          /* base‑3 digit (0,1,2) */
        res += (seq_t)d * p;         /* add digit · (-3)^k to result */
        p = p * (seq_t)(-3);         /* advance to the next power: (-3)^{k+1} */
        x /= 3;                      /* move to next digit */
    }

    return res;
}
