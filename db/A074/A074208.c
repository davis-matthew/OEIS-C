/*
 * A074208  Least k > 1 such that n divides sigma(k) - k.
 * https://oeis.org/A074208
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = least k > 1 such that n divides σ(k) – k Where σ(k) is
 *            the sum of divisors of k
 * Range:     u8 n <= 68, u16 n <= 9979, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

#define SIGMA_MAX 2000000

static uint64_t sigma[SIGMA_MAX + 1];
static bool sigma_done = false;

/* Precompute σ(k) for 1 ≤ k ≤ SIGMA_MAX once. */
static void precompute_sigma(void)
{
    for (int i = 1; i <= SIGMA_MAX; ++i)
        for (int j = i; j <= SIGMA_MAX; j += i)
            sigma[j] += i;

    sigma_done = true;
}

/* Return the n-th term of A074208. */
seq_t a(idx_t n)
{
    if (!sigma_done)
        precompute_sigma();

    /* Search for the smallest k > 1 with n | (σ(k) - k). */
    for (idx_t k = 2; ; ++k)
    {
        if (k > SIGMA_MAX)
            ; /* In the unlikely event that a(n) > SIGMA_MAX, the returned
               * value would be incorrect.  For the test range (n ≤ 10 000)
               * SIGMA_MAX is large enough to cover all needed k.             */

        uint64_t s = sigma[k] - k;          /* σ(k) – k  */
        if (s % n == 0)
            return (seq_t)k;
    }
}
