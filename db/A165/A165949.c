/*
 * A165949  a(n) = A027762(n)/A165734(n).
 * https://oeis.org/A165949
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{d|n, d≥3, 2*d+1 is prime} (2*d+1)
 * Range:     u8 n <= 14, u16 n <= 29, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   seq_mul, seq_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_one.h"
#include "../utils/seq_mul.h"

#define MAXP 20001  /* 2*10000 + 1   (covers all 2*d+1 needed for n≤10000) */

static unsigned char primes[MAXP + 1];
static bool primes_initialized = false;

static void init_primes(void)
{
    /* Sieve of Eratosthenes up to MAXP */
    for (size_t i = 0; i <= MAXP; ++i)
        primes[i] = 1;
    primes[0] = primes[1] = 0;
    for (size_t i = 2; i * i <= MAXP; ++i) {
        if (primes[i]) {
            for (size_t j = i * i; j <= MAXP; j += i)
                primes[j] = 0;
        }
    }
    primes_initialized = true;
}

seq_t a(idx_t n)
{
    if (!primes_initialized)
        init_primes();

    seq_t prod = seq_one();
    if (n < 3)          /* n = 1 or 2 → product stays 1 */
        return prod;

    /* Enumerate divisors d of n with 2 ≤ d ≤ √n   (using n/d to avoid overflow) */
    for (idx_t d = 2; d <= n / d; ++d) {
        if (n % d == 0) {
            /* divisor d itself, if ≥3 */
            if (d >= 3) {
                idx_t p = 2 * d + 1;
                if (p <= MAXP && primes[p])
                    prod = seq_mul(prod, (seq_t)p);
            }

            /* paired divisor n/d, distinct from d and ≥3 */
            idx_t d2 = n / d;
            if (d2 != d && d2 >= 3) {
                idx_t p = 2 * d2 + 1;
                if (p <= MAXP && primes[p])
                    prod = seq_mul(prod, (seq_t)p);
            }
        }
    }

    /* Handle the divisor n itself (not covered by the loop if n>1) */
    if (n >= 3) {
        idx_t p = 2 * n + 1;
        if (p <= MAXP && primes[p])
            prod = seq_mul(prod, (seq_t)p);
    }

    return prod;
}
