/*
 * A053150  Cube root of largest cube dividing n.
 * https://oeis.org/A053150
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, mult
 * Formula:   a(n) = product over prime powers p^e in n of p^(floor(e/3))
 *            Equivalent: the largest integer k such that k^3 divides n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

static uint32_t primes[80000];
static size_t primes_cnt = 0;
static bool primes_initialized = false;

/* Simple sieve to populate the prime list up to 1,000,000.  */
static void init_primes(void) {
    if (primes_initialized) return;
    const uint32_t LIMIT = 1000000;
    bool *is_composite = (bool*)malloc((LIMIT + 1) * sizeof(bool));
    if (!is_composite) return;  /* allocation failure fallback */

    for (uint32_t i = 2; i <= LIMIT; ++i) {
        if (!is_composite[i]) {
            if (primes_cnt < 80000) primes[primes_cnt++] = i;
            if ((uint64_t)i * i <= LIMIT) {
                for (uint32_t j = i * i; j <= LIMIT; j += i) {
                    is_composite[j] = true;
                }
            }
        }
    }
    free(is_composite);
    primes_initialized = true;
}

/* Compute a(n): largest integer whose cube divides n. */
seq_t a(idx_t n) {
    if (n <= 1) return (seq_t)1;   /* a(1) = 1, a(0) not defined but return 1 */
    init_primes();

    uint64_t m = (uint64_t)n;
    seq_t result = (seq_t)1;

    for (size_t i = 0; i < primes_cnt && (uint64_t)primes[i] * primes[i] <= m; ++i) {
        uint32_t p = primes[i];
        unsigned exp = 0;
        while (m % p == 0) {
            ++exp;
            m /= p;
        }
        if (exp >= 3) {
            unsigned q = exp / 3;   /* floor(exp/3) */
            seq_t pow = (seq_t)1;
            for (unsigned j = 0; j < q; ++j) pow *= (seq_t)p;
            result *= pow;
        }
    }

    /* Remaining m > 1 consists only of prime factors with exponent 1,
       so floor(1/3) == 0 and contributes nothing.  */
    return result;
}
