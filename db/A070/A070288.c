/*
 * A070288  a(n) = Sum_{d|n} omega(d)*omega(n/d).
 * https://oeis.org/A070288
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{d|n} omega(d)*omega(n/d)
 * Range:     u8 n <= 4619, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where omega(m) = number of distinct prime factors of m */

#include <stdint.h>
#include <stdbool.h>

/* Maximum number of distinct prime factors for n up to typical limits
   (enough for any n up to 10^12 and more). */
#define MAX_PRIMES 64

seq_t a(idx_t n) {
    /* Factor n, storing distinct primes and their exponents */
    uint64_t temp = (uint64_t)n;
    uint64_t primes[MAX_PRIMES];
    uint32_t exp[MAX_PRIMES];
    int k = 0;

    if (temp == 0) return 0;          /* defensive, n>=1 by spec */
    if (temp >= 2) {
        if (temp % 2 == 0) {
            uint32_t e = 0;
            while (temp % 2 == 0) {
                temp /= 2;
                e++;
            }
            primes[k] = 2;
            exp[k] = e;
            k++;
        }
        for (uint64_t p = 3; p <= temp / p; p += 2) {
            if (temp % p == 0) {
                uint32_t e = 0;
                while (temp % p == 0) {
                    temp /= p;
                    e++;
                }
                primes[k] = p;
                exp[k] = e;
                k++;
            }
        }
        if (temp > 1) {   /* remaining prime factor > sqrt(original n) */
            primes[k] = temp;
            exp[k] = 1;
            k++;
        }
    }

    /* If n==1, k==0 and the answer is 0 */
    if (k == 0) return 0;

    /* Compute tau(n) = product (exp[i] + 1) */
    seq_t tau = 1;
    for (int i = 0; i < k; i++)
        tau *= (seq_t)(exp[i] + 1);

    /* Sum contributions for all ordered pairs of primes */
    seq_t sum = 0;
    for (int i = 0; i < k; i++) {
        seq_t prod_i = tau / (seq_t)(exp[i] + 1);      /* product of (exp[j]+1) for j != i */
        /* Self-pair contribution: (a_i-1) * prod_i */
        if (exp[i] >= 1)
            sum += (seq_t)(exp[i] - 1) * prod_i;

        for (int j = 0; j < k; j++) {
            if (i == j) continue;
            /* Ordered pair contribution: a_i * a_j * prod_{i,j} */
            seq_t prod_ij = tau / (seq_t)((exp[i] + 1) * (exp[j] + 1));
            sum += (seq_t)exp[i] * (seq_t)exp[j] * prod_ij;
        }
    }
    return sum;
}
