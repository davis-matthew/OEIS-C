/*
 * A062731  Sum of divisors of 2*n.
 * https://oeis.org/A062731
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sigma(2*n)
 * Range:     u8 n <= 53, u16 n <= 8819, u32 n <= 9034, u64 n <= 9034,
 *            u128 n <= 9034, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t m = (idx_t)2 * n;          /* m = 2*n */
    seq_t result = (seq_t)1;         /* product over prime power sums */

    /* factor m */
    if ((m & 1) == 0) {              /* handle factor 2 */
        idx_t exp = 0;
        while ((m & 1) == 0) {       /* while m divisible by 2 */
            m >>= 1;
            exp++;
        }
        seq_t sum = (seq_t)1;        /* 1 + 2 + 2^2 + ... */
        seq_t pow = (seq_t)1;
        for (idx_t i = 0; i < exp; ++i) {
            pow *= (seq_t)2;
            sum += pow;
        }
        result *= sum;
    }

    /* odd prime factors, try 3,5,7,... */
    for (idx_t p = 3; p <= (m / p); p += 2) {
        if (m % p == 0) {
            idx_t exp = 0;
            while (m % p == 0) {
                m /= p;
                exp++;
            }
            seq_t sum = (seq_t)1;
            seq_t pow = (seq_t)1;
            for (idx_t i = 0; i < exp; ++i) {
                pow *= (seq_t)p;
                sum += pow;
            }
            result *= sum;
        }
    }

    if (m > 1) {                     /* remaining prime factor */
        seq_t sum = (seq_t)1 + (seq_t)m;
        result *= sum;
    }

    return result;
}
