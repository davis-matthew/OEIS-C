/*
 * A056839  Squarefree part of lcm(1,...,n).
 * https://oeis.org/A056839
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{p prime ≤ n} p^{⌊log_p n⌋ – 2·⌊log_p ⌊√n⌋⌋}
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 30, u64 n <= 58, u128 n <= 106,
 *            big any n
 * Helpers:   int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This equals lcm(1..n) divided by lcm(1..⌊√n⌋)^2. */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/int_sqrt.h"

seq_t a(idx_t n)
{
    /* Handle the trivial cases n = 0 or 1 directly. */
    if (n <= 1)
        return 1;

    /* Number of primes needed is at most n. Allocate sieve boolean array. */
    size_t limit = (size_t)n;                 /* n fits into size_t for limits ≤ 10⁴ */
    unsigned char *is_comp = (unsigned char *)calloc(limit + 1, 1);
    if (!is_comp)
        return 0; /* unlikely on small n, but defensive */

    idx_t q = int_sqrt(n);          /* floor(sqrt(n)) */

    seq_t result = 1;

    /* Sieve of Eratosthenes – iterate over possible primes p ≤ n. */
    for (size_t p = 2; p <= limit; ++p) {
        if (is_comp[p])
            continue;              /* composite, skip */

        /* Mark multiples of this prime as composite. */
        if (p * p <= limit) {
            for (size_t j = p * p; j <= limit; j += p)
                is_comp[j] = 1;
        }

        /* Compute exponents: floor(log_p n) and floor(log_p q). */
        uint64_t t = (uint64_t)p;
        uint64_t e_n = 0, e_q = 0;

        while (t <= n) {
            ++e_n;
            if (t > n / p) break;
            t *= p;
        }

        t = (uint64_t)p;
        while (t <= q) {
            ++e_q;
            if (t > q / p) break;
            t *= p;
        }

        /* Difference of exponents for square‑free part. */
        long long diff = (long long)e_n - 2LL * (long long)e_q;
        if (diff > 0) {
            /* Multiply result by p^diff. */
            for (long long i = 0; i < diff; ++i)
                result *= (seq_t)p;
        }
    }

    free(is_comp);
    return result;
}
