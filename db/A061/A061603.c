/*
 * A061603  a(n) = n! / {product of factorials of the digits of n}.
 * https://oeis.org/A061603
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n! / (product of factorials of digits of n)
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 14, u64 n <= 20, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

static inline seq_t pow_seq_t(seq_t base, int exp)
{
    seq_t result = (seq_t)1;
    while (exp) {
        if (exp & 1) result *= base;
        if (exp > 1) base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Memoization state */
    static bool initialized = false;
    static const int MAX_PRIME_CAP = 10000;               /* primes up to 10 000        */
    static int primes[2000];                             /* adequate for 10 000       */
    static size_t prime_cnt = 0;
    static int digit_exp[10][4];                         /* exponents of 2,3,5,7 in d! */

    if (!initialized) {
        /* ---- sieve of Eratosthenes (up to MAX_PRIME_CAP) ---- */
        int is_comp[MAX_PRIME_CAP + 1];
        for (int i = 0; i <= MAX_PRIME_CAP; ++i) is_comp[i] = 0;
        for (int p = 2; p <= MAX_PRIME_CAP; ++p) {
            if (!is_comp[p]) {
                primes[prime_cnt++] = p;
                if (p > MAX_PRIME_CAP / p) continue;
                for (int m = p * p; m <= MAX_PRIME_CAP; m += p)
                    is_comp[m] = 1;
            }
        }

        /* Pre‑compute digit factorial exponents for primes 2,3,5,7 */
        for (int d = 0; d <= 9; ++d) {
            for (int pi = 0; pi < 4; ++pi) {
                int p = primes[pi];
                if (p > d) {
                    digit_exp[d][pi] = 0;
                    continue;
                }
                int t = d, cnt = 0;
                while (t) {
                    t /= p;
                    cnt += t;
                }
                digit_exp[d][pi] = cnt;
            }
        }
        initialized = true;
    }

    /* trivial cases */
    if (n <= 1) return (seq_t)1;          /* 0! = 1, 1! = 1 */

    /* exponents of all primes in n! */
    int exp[2000] = {0};
    for (size_t pi = 0; pi < prime_cnt; ++pi) {
        int p = primes[pi];
        int e = 0;
        idx_t div = (idx_t)p;
        while (div <= n) {
            e += (int)(n / div);
            if (div > n / (idx_t)p) break;  /* avoid overflow of div * p   */
            div *= (idx_t)p;
        }
        exp[pi] = e;
    }

    /* subtract contributions of factorials of the decimal digits of n      */
    idx_t t = n;
    while (t > 0) {
        int d = (int)(t % 10);
        for (int pi = 0; pi < 4; ++pi)   /* primes[0..3] are 2,3,5,7       */
            exp[pi] -= digit_exp[d][pi];
        t /= 10;
    }

    /* reconstruct the result from prime exponents                 */
    seq_t result = (seq_t)1;
    for (size_t pi = 0; pi < prime_cnt; ++pi) {
        int e = exp[pi];
        if (e <= 0) continue;
        seq_t p_power = pow_seq_t((seq_t)primes[pi], e);
        result *= p_power;
    }
    return result;
}
