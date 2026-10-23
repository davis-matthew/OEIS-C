/*
 * A168036  Difference between n' and n, where n' is the arithmetic derivative
 *          of n (A003415).
 * https://oeis.org/A168036
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = n' - n, where n' is the arithmetic derivative of n
 *            (A003415)
 * Range:     i8 n <= 63, i16 n <= 8191, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t arithmetic_derivative(idx_t n)
{
    /* n' = n * sum_{p^e||n} e / p  = sum_{p^e||n} (n/p) * e   */
    if (n <= 1)                /* 0' = 0, 1' = 0 */
        return 0;
    seq_t nseq = (seq_t)n;
    seq_t deriv = 0;
    idx_t tmp = n;

    /* factor 2 */
    if (tmp % 2 == 0) {
        int e = 0;
        while (tmp % 2 == 0) {
            tmp /= 2;
            e++;
        }
        deriv += (nseq / 2) * e;          /* (n/2)*e */
    }

    /* factor odd primes */
    for (idx_t p = 3; p * p <= tmp; p += 2) {
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                e++;
            }
            deriv += (nseq / p) * e;      /* (n/p)*e */
        }
    }

    /* remaining prime factor > sqrt(original n) */
    if (tmp > 1) {
        deriv += (nseq / tmp);            /* e = 1 */
    }

    return deriv;
}

seq_t a(idx_t n)
{
    /* offset 0: a(0)=0, a(1)=-1, ...      */
    seq_t der = arithmetic_derivative(n);
    return der - (seq_t)n;
}
