/*
 * A100006  Integer log of 2n: sum of primes dividing 2n (with repetition).
 * https://oeis.org/A100006
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum of primes (with multiplicity) dividing (2*n)
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    /* Work with unsigned 64‑bit for intermediate factorisation */
    uint64_t x   = (uint64_t)n;
    x <<= 1;                          /* x = 2*n */

    uint64_t tmp = x;

    /* factor out 2 */
    while ((tmp & 1ULL) == 0) {       /* tmp % 2 == 0 */
        sum += (seq_t)2;
        tmp >>= 1;                    /* tmp /= 2 */
    }

    /* factor odd divisors */
    for (uint64_t p = 3; p <= tmp / p; p += 2) {
        while (tmp % p == 0) {
            sum += (seq_t)p;
            tmp /= p;
        }
    }

    /* if a prime factor larger than sqrt remains */
    if (tmp > 1) {
        sum += (seq_t)tmp;
    }

    return sum;
}
