/*
 * A228425  Number of ways to write n = x + y (x, y > 0) with x*(x+1)/2 + y^2
 *          prime.
 * https://oeis.org/A228425
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of pairs (x, y > 0) with x + y = n such that * T_x
 *            + y^2 is prime, where T_x = x*(x+1)/2 is the x-th triangular
 *            number
 * Range:     u8 n <= 16, u16 n <= 1024, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

static void init_primes(int *primes, size_t *count)
{
    static bool done = 0;
    if (done) return;
    const int LIM    = 20000;          /* 20000² = 400 000 000 > max sum for n ≤ 10000   */
    char sieve[LIM + 1];
    for (int i = 0; i <= LIM; ++i) sieve[i] = 0;
    sieve[0] = sieve[1] = 1;
    for (int i = 2; i * i <= LIM; ++i)
        if (!sieve[i])
            for (int j = i * i; j <= LIM; j += i)
                sieve[j] = 1;

    *count = 0;
    for (int i = 2; i <= LIM; ++i)
        if (!sieve[i])
            primes[(*count)++] = i;
    done = 1;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* Indexing starts at 1 ‑ defined by OEIS offset */

    /* Prime table – static so it is built only once */
    static int primes[3000];
    static size_t prime_count = 0;
    init_primes(primes, &prime_count);

    seq_t count = 0;

    for (idx_t x = 1; x < n; ++x) {
        idx_t y = n - x;

        seq_t tri = (seq_t)x * ((seq_t)x + 1) / 2;
        seq_t sum = tri + (seq_t)y * (seq_t)y;

        bool prime = true;
        if (sum < (seq_t)2)
            prime = false;
        else {
            for (size_t i = 0; i < prime_count; ++i) {
                int p = primes[i];
                seq_t pp = (seq_t)p * (seq_t)p;
                if (pp > sum) break;
                if (sum % (seq_t)p == 0) {
                    prime = false;
                    break;
                }
            }
        }

        if (prime) ++count;
    }

    return count;
}
