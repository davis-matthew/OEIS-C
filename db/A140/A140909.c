/*
 * A140909  Number of 5 X 7 matrices with elements in 0..n with each row and
 *          each column in nondecreasing order. 5,7,n can be permuted, see
 *          formula.
 * https://oeis.org/A140909
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   uses MacMahon's formula for the number of plane partitions in a
 *            5×7 box with entries ≤ n. The result equals a(n) =
 *            ∏_{i=1}^{5}∏_{j=1}^{7}∏_{k=1}^{n} (i+j+k-1)/(i+j+k-2). The
 *            computation is performed by factorising each factor, keeping
 *            prime exponents, and finally multiplying the prime powers
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h> /* for memset */
#include "../utils/big_pow.h" /* for seq_t big_pow(seq_t, seq_t) */

static int primes[20000];
static int primeCount = 0;
static int primeToIndex[20001];
static int init_flag = 0;

static void init_primes(void)
{
    /* Sieve primes up to 20000 (enough for n up to ~10000). */
    int maxVal = 20000;
    uint8_t isPrime[20001];
    memset(isPrime, 1, sizeof(isPrime));
    isPrime[0] = isPrime[1] = 0;
    for (int i = 2; i * i <= maxVal; ++i) {
        if (isPrime[i]) {
            for (int j = i * i; j <= maxVal; j += i)
                isPrime[j] = 0;
        }
    }
    for (int i = 2; i <= maxVal; ++i) {
        if (isPrime[i]) {
            primes[primeCount++] = i;
        }
    }
    /* Build mapping from prime to index. */
    for (int i = 0; i <= maxVal; ++i)
        primeToIndex[i] = -1;
    for (int i = 0; i < primeCount; ++i)
        primeToIndex[primes[i]] = i;
    init_flag = 1;
}

seq_t a(idx_t n)
{
    if (!init_flag)
        init_primes();

    /* Array to hold prime exponents; size at most primeCount. */
    int expArray[20000];
    for (int i = 0; i < primeCount; ++i)
        expArray[i] = 0;

    /* Loop over all factors and factorise them. */
    for (idx_t i = 1; i <= 5; ++i) {
        for (idx_t j = 1; j <= 7; ++j) {
            for (idx_t k = 1; k <= n; ++k) {
                int num = (int)(i + j + k - 1); /* numerator factor */
                int den = (int)(i + j + k - 2); /* denominator factor */

                /* factorise numerator */
                int temp = num;
                for (int pi = 0; pi < primeCount && primes[pi] * primes[pi] <= temp; ++pi) {
                    int p = primes[pi];
                    if (temp % p == 0) {
                        int cnt = 0;
                        while (temp % p == 0) {
                            temp /= p;
                            ++cnt;
                        }
                        expArray[pi] += cnt;
                    }
                }
                if (temp > 1) {
                    int index = primeToIndex[temp];
                    expArray[index] += 1;
                }

                /* factorise denominator */
                temp = den;
                for (int pi = 0; pi < primeCount && primes[pi] * primes[pi] <= temp; ++pi) {
                    int p = primes[pi];
                    if (temp % p == 0) {
                        int cnt = 0;
                        while (temp % p == 0) {
                            temp /= p;
                            ++cnt;
                        }
                        expArray[pi] -= cnt;
                    }
                }
                if (temp > 1) {
                    int index = primeToIndex[temp];
                    expArray[index] -= 1;
                }
            }
        }
    }

    /* Multiply the prime powers to obtain the result. */
    seq_t result = (seq_t)1;
    for (int pi = 0; pi < primeCount; ++pi) {
        int e = expArray[pi];
        if (e > 0) {
            seq_t term = big_pow((seq_t)primes[pi], (seq_t)e);
            result = result * term;
        }
    }
    return result;
}
