/*
 * A086047  Sum of first n 5-almost primes.
 * https://oeis.org/A086047
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 97, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = sum of the first n integers whose Ω(n)=5
   (i.e. the sum of the first n 5‑almost primes). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Build the prefix array only once. */
    static bool built      = false;
    static seq_t *prefix   = NULL;
    static idx_t  count5   = 0;
    const int LIMIT = 4000000; /* enough to contain >10000 5‑almost primes */

    if (!built) {
        /* Linear sieve to compute smallest prime factor and Ω(i). */
        int *spf   = (int *)malloc((LIMIT + 1) * sizeof(int));
        unsigned char *omega = (unsigned char *)malloc((LIMIT + 1) * sizeof(unsigned char));
        int *primes   = (int *)malloc((LIMIT / 10 + 1) * sizeof(int));
        int pcnt = 0;

        spf[0] = 0;
        spf[1] = 1;
        omega[0] = 0;
        omega[1] = 0;

        for (int i = 2; i <= LIMIT; ++i) {
            if (spf[i] == 0) {           /* prime */
                spf[i] = i;
                primes[pcnt++] = i;
                omega[i] = 1;
            } else {
                omega[i] = omega[i / spf[i]] + 1;
            }
            for (int j = 0; j < pcnt; ++j) {
                int p = primes[j];
                long long x = (long long)p * i;
                if (x > LIMIT) break;
                spf[(int)x] = p;
                if (p == spf[i]) break;
            }
        }

        /* Count how many 5‑almost primes we have. */
        for (int i = 2; i <= LIMIT; ++i)
            if (omega[i] == 5)
                ++count5;

        /* Allocate prefix sum array.  prefix[0] = 0; prefix[1] = first prime. */
        prefix = (seq_t *)malloc((count5 + 1) * sizeof(seq_t));
        prefix[0] = (seq_t)0;

        idx_t pos = 0;
        for (int i = 2; i <= LIMIT; ++i) {
            if (omega[i] == 5) {
                ++pos;
                prefix[pos] = prefix[pos - 1] + (seq_t)i;
            }
        }

        /* Clean up temporary arrays. */
        free(spf);
        free(omega);
        free(primes);

        built = true;
    }

    if (n > count5) return (seq_t)0;   /* out of range – should not happen */
    return prefix[(size_t)n];
}
