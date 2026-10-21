/*
 * A087688  a(n) = number of solutions to x^3 - x == 0 (mod n).
 * https://oeis.org/A087688
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, nonn, easy
 * Formula:   a(n) = number of solutions to x^3 ≡ x (mod n); a(n) is
 *            multiplicative. For odd prime powers p^e, a(p^e) = 3. For power
 *            of 2: a(2) = 2, a(4) = 3, a(2^e) = 5 for e ≥ 3
 * Range:     u8 n <= 9239, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    seq_t result = (seq_t)1;
    idx_t tmp = n;

    /* handle factor 2 */
    if ((tmp & 1) == 0) {          /* tmp % 2 == 0 */
        idx_t e = 0;
        while ((tmp & 1) == 0) {   /* tmp % 2 == 0 */
            tmp >>= 1;              /* tmp /= 2 */
            e++;
        }
        if (e == 1)
            result *= (seq_t)2;
        else if (e == 2)
            result *= (seq_t)3;
        else
            result *= (seq_t)5;
    }

    /* handle odd prime factors */
    for (idx_t p = 3; p * p <= tmp; p += 2) {
        if (tmp % p == 0) {
            while (tmp % p == 0)
                tmp /= p;
            result *= (seq_t)3;
        }
    }

    /* remaining prime factor > sqrt(tmp) */
    if (tmp > 1)
        result *= (seq_t)3;

    return result;
}
