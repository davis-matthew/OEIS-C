/*
 * A074376  s(3s-1)/2 where s is the sum of the prime factors of n (with
 *          repetition).
 * https://oeis.org/A074376
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = s*(3*s-1)/2 where s is the sum of the prime factors of n
 *            (with repetition)
 * Range:     u8 n <= 16, u16 n <= 210, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t nn = n;          /* work on a copy of n */
    seq_t s = 0;           /* sum of prime factors */

    /* factor out 2 first */
    while ((nn & 1ULL) == 0) {
        s += 2;
        nn >>= 1;          /* divide by 2 */
    }

    /* trial divide by odd primes up to sqrt(nn) */
    for (idx_t p = 3; p * p <= nn; p += 2) {
        while (nn % p == 0) {
            s += (seq_t)p;
            nn /= p;
        }
    }

    /* if anything remains it is a prime factor > sqrt(original n) */
    if (nn > 1) {
        s += (seq_t)nn;
    }

    /* compute the sequence value: s*(3*s-1)/2 */
    seq_t three_s_minus1 = (seq_t)3 * s - 1;
    return s * three_s_minus1 / 2;
}
