/*
 * A160907  Sum of the number of prime divisors including repetitions between
 *          n^2 and (n+1)^2.
 * https://oeis.org/A160907
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Σ_{m=n^2}^{(n+1)^2} Ω(m) where Ω(m) is the number of
 *            prime divisors of m counted with multiplicity. The function
 *            builds a table of primes up to 11000 (enough for any n ≤ 10 000)
 *            once and then uses trial division to count Ω(m) for each integer
 *            in the required range
 * Range:     u8 n <= 39, u16 n <= 8274, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* --------- prime table, built once ------------------------------------ */
static bool primes_built = false;
static uint32_t primes[20000];
static size_t primes_cnt = 0;

/* Build primes up to 11000 using a simple sieve.  */
static void build_primes(void)
{
    if (primes_built) return;
    const uint32_t LIMIT = 11000;
    bool *is_composite = (bool*)calloc(LIMIT + 1, 1);
    if (!is_composite) exit(EXIT_FAILURE);
    primes_cnt = 0;
    for (uint32_t i = 2; i <= LIMIT; ++i) {
        if (!is_composite[i]) {
            primes[primes_cnt++] = i;
            if ((uint64_t)i * i <= LIMIT) {
                for (uint32_t j = i * i; j <= LIMIT; j += i)
                    is_composite[j] = true;
            }
        }
    }
    free(is_composite);
    primes_built = true;
}

/* Count prime divisors (with multiplicity) of m using the prime table. */
static seq_t count_omega(uint64_t m)
{
    seq_t cnt = 0;
    uint64_t v = m;
    for (size_t i = 0; i < primes_cnt && (uint64_t)primes[i] * primes[i] <= v; ++i) {
        uint32_t p = primes[i];
        while (v % p == 0) {
            ++cnt;
            v /= p;
        }
    }
    if (v > 1) ++cnt;   /* remaining prime factor */
    return cnt;
}

/* --------- sequence function ------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* outside offset, defined to return 0 */

    build_primes();

    idx_t lower = n * n;
    idx_t upper = (n + 1) * (n + 1);

    seq_t sum = 0;
    for (idx_t m = lower; m <= upper; ++m) {
        sum += count_omega((uint64_t)m);
    }
    return sum;
}
