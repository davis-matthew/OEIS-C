/*
 * A146100  Bell numbers (A000110) read mod 10.
 * https://oeis.org/A146100
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = B(n) mod 10, where B(n) is the nth Bell number. The
 *            sequence is periodic modulo 10 with period 2343. The recurrence
 *            used is B(0) = 1, B(n+1) = Σ_{k=0}^{n} C(n,k) · B(k) (mod 10)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

#define PERIOD 2343U

static bool initialized = false;
static seq_t cache[PERIOD];     /* B(0)…B(PERIOD-1) modulo 10 */

static void init_cache(void)
{
    if (initialized) return;

    cache[0] = (seq_t)1;           /* B(0) = 1 */

    static int binom[PERIOD];      /* C(n,k) modulo 10, current row */
    binom[0] = 1;                  /* first row of Pascal's triangle */

    for (idx_t n = 0; n < PERIOD - 1; ++n) {
        int val = 0;
        /* Compute B(n+1) = Σ_{k=0}^{n} C(n,k)*B(k) (mod 10) */
        for (idx_t k = 0; k <= n; ++k) {
            val += binom[(size_t)k] * (int)cache[(size_t)k];
        }
        val %= 10;
        cache[(size_t)(n + 1)] = (seq_t)val;

        /* Advance to next row of binomial coefficients:
           C(n+1,k) = C(n,k) + C(n,k-1) (mod 10)   for k ≥ 1 */
        for (idx_t k = n + 1; k > 0; --k) {
            binom[(size_t)k] = (binom[(size_t)k] + binom[(size_t)(k - 1)]) % 10;
        }
        /* binom[0] stays 1 */
    }

    initialized = true;
}

seq_t a(idx_t n)
{
    init_cache();
    idx_t mod = n % PERIOD;
    return cache[(size_t)mod];
}
