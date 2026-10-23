/*
 * A128558  a(n) = the number of positive divisors of (n!/A128557(n)!).
 * https://oeis.org/A128558
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the divisor count of the product of the factors from some
 *            k+1 to n where k is the largest integer < n such that this
 *            divisor count is minimal and >= n. In other words, a(n) is the
 *            first divisor count that reaches or exceeds n when multiplying
 *            successive integers downwards from n
 * Range:     u8 n <= 62, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <string.h>

#define MAX_N 10000            /* maximum n the sequence will be asked for */

/* Smallest prime factor for each number up to MAX_N */
static uint16_t spf[MAX_N + 1];

/* Initialize spf array once */
static void init_spf(void) {
    static int done = 0;
    if (done) return;
    for (int i = 0; i <= MAX_N; ++i)
        spf[i] = 0;
    spf[0] = 0;
    spf[1] = 1;
    for (int i = 2; i <= MAX_N; ++i) {
        if (spf[i] == 0) {          /* i is prime */
            spf[i] = (uint16_t)i;
            if ((long long)i * i <= MAX_N) {
                for (int j = i * i; j <= MAX_N; j += i)
                    if (spf[j] == 0)
                        spf[j] = (uint16_t)i;
            }
        }
    }
    done = 1;
}

/* Exponent array for primes, reused each call. */
static uint16_t prime_exp[MAX_N + 1];

/* Main sequence function */
seq_t a(idx_t n)
{
    init_spf();

    if (n < 1) return 0;          /* out of range – should not happen */
    if (n == 1) return 1;

    /* ensure we only need exponents up to n */
    if (n > MAX_N) {
        /* for safety; the problem guarantees n <= 10000 */
        return 0;
    }

    /* Reset exponents for primes up to n */
    memset(prime_exp, 0, (n + 1) * sizeof(uint16_t));

    seq_t tau = (seq_t)1;         /* current divisor count of the suffix product */

    for (idx_t cur = n; cur >= 1 && tau < (seq_t)n; --cur) {
        idx_t x = cur;
        while (x > 1) {
            idx_t p = spf[x];
            uint32_t cnt = 0;
            while (x % p == 0) {
                x /= p;
                ++cnt;
            }
            uint32_t old = prime_exp[p];
            uint32_t newe = old + cnt;
            /* tau = tau/(old+1)*(newe+1)   -- all operations on seq_t */
            tau = (tau / (seq_t)(old + 1)) * (seq_t)(newe + 1);
            prime_exp[p] = (uint16_t)newe;
        }
    }

    return tau;
}
