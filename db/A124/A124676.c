/*
 * A124676  a(n) = largest integer < n such that the positive integers <= a(n)
 *          and coprime to a(n) are also coprime to n.
 * https://oeis.org/A124676
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest integer < n such that every integer k ≤ a(n) that
 *            is coprime to a(n) is also coprime to n. The function scans
 *            downward from n‑1 and checks the prime–divisor condition.
 *            Author: ChatGPT
 * Range:     u8 n <= 256, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* Off‑by‑offset: a(2) = 1 */
    if (n <= 2)
        return (seq_t)1;

    /* Prime factorisation of n (distinct primes only) */
    uint64_t primes[64];
    uint64_t exps[64];          /* exponents are not used */
    size_t   pcnt;
    factor_u64(n, primes, exps, &pcnt);

    /* Search for the largest k < n satisfying the condition */
    for (idx_t k = n - 1; k > 0; --k)
    {
        bool ok = true;
        for (size_t i = 0; i < pcnt; ++i)
        {
            uint64_t p = primes[i];
            if (p <= (uint64_t)k && (k % p) != 0)
            {
                ok = false;
                break;
            }
        }
        if (ok)
            return (seq_t)k;
    }

    /* Fallback – should never be reached for n ≥ 2 */
    return (seq_t)1;
}
