/*
 * A156745  a(n) = Sum_{k=1..n} floor((n+k)/k) = n + Sum_{k=1..n} sigma_0(k),
 *          where sigma_0(k) is A000005(k). Also a(n) = n + A006218(n).
 * https://oeis.org/A156745
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n + ∑_{k=1..n} σ₀(k) (σ₀ = number of divisors)
 * Range:     u8 n <= 49, u16 n <= 6587, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 73 terms through n = 8719 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset 1 – not used in tests */

    /* Count divisors for all numbers ≤ n using a sieve‑like update. */
    seq_t *divcnt = mem_new(seq_t, (n + 1));
    for (idx_t i = 0; i <= n; ++i) divcnt[i] = 0;

    for (idx_t i = 1; i <= n; ++i) {
        for (idx_t j = i; j <= n; j += i) {
            divcnt[j] += (seq_t)1;
        }
    }

    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) sum += divcnt[k];

    mem_free(divcnt);
    return (seq_t)n + sum;
}
