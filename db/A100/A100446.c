/*
 * A100446  Bisection of A000031.
 * https://oeis.org/A100446
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/(2n))*∑_{d|2n} φ(d)*2^{(2n)/d} where φ is Euler's
 *            totient function
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

#define MAXM 20000   /* 2*10000 */

static seq_t pow2[MAXM + 1];
static uint32_t phi[MAXM + 1];
static int initialized = 0;

static void init_tables(void) {
    /* Totient sieve */
    for (size_t i = 0; i <= MAXM; ++i)
        phi[i] = (uint32_t)i;
    for (size_t i = 2; i <= MAXM; ++i) {
        if (phi[i] == i) {                 /* i is prime */
            for (size_t j = i; j <= MAXM; j += i)
                phi[j] -= phi[j] / (uint32_t)i;
        }
    }

    /* Powers of two */
    pow2[0] = (seq_t)1;
    for (size_t i = 1; i <= MAXM; ++i)
        pow2[i] = pow2[i - 1] * (seq_t)2;
}

seq_t a(idx_t n) {
    if (!initialized) {
        init_tables();
        initialized = 1;
    }

    if (n == 0) return (seq_t)1;

    idx_t m = n * ((idx_t)2);      /* m = 2n */

    seq_t sum = 0;
    for (idx_t i = 1; i * i <= m; ++i) {
        if (m % i == 0) {
            idx_t d1 = i;
            idx_t d2 = m / i;

            seq_t term1 = (seq_t)phi[d1] * pow2[m / d1];
            sum += term1;

            if (d2 != d1) {
                seq_t term2 = (seq_t)phi[d2] * pow2[m / d2];
                sum += term2;
            }
        }
    }

    return sum / (seq_t)m;
}
