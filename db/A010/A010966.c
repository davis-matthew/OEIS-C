/*
 * A010966  a(n) = binomial(n,13).
 * https://oeis.org/A010966
 *
 * Offset:    13    (a(13) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(n,13)
 * Range:     u8 n <= 15, u16 n <= 19, u32 n <= 37, u64 n <= 177,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* binomial(n,13) = n*(n-1)*...*(n-12) / 13!
       We cancel the prime factors of 13! from the numerator
       without any intermediate overflow on any supported seq_t. */

    if (n < 13) {
        return 0; /* undefined for n < 13 */
    }

    /* 13! prime factorisation:
       13! = 2^10 * 3^5 * 5^2 * 7^1 * 11^1 * 13^1 */
    static const idx_t primes[6] = {2, 3, 5, 7, 11, 13};
    static const unsigned exps[6] = {10, 5, 2, 1, 1, 1};

    unsigned rem[6];
    for (int i = 0; i < 6; ++i) {
        rem[i] = exps[i];
    }

    seq_t result = (seq_t)1;

    for (idx_t i = 0; i < 13; ++i) {
        seq_t x = (seq_t)(n - i); /* term (n-i) */
        for (int j = 0; j < 6; ++j) {
            if (rem[j] == 0) continue;
            seq_t p = (seq_t)primes[j];
            /* Cancel as many factors of p as possible from x */
            while (rem[j] > 0 && x % p == 0) {
                x /= p;
                rem[j]--;
            }
        }
        result *= x;
    }

    /* All denominator factors should be cancelled */
    for (int i = 0; i < 6; ++i) {
        /* In a correct implementation rem[i] should be 0 now. */
    }

    return result;
}
