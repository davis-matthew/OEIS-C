/*
 * A099905  a(n) = binomial(2n-1, n-1) mod n.
 * https://oeis.org/A099905
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(2*n-1, n-1) mod n
 * Range:     u8 n <= 308, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   exp_in_fact, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/exp_in_fact.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t mod = (seq_t)n;
    seq_t result = 1;

    /* Generate primes up to 2*n-1 */
    idx_t limit = 2*n - 1;
    bool *prime_flag = (bool*)malloc((size_t)(limit + 1) * sizeof(bool));
    if (!prime_flag) return 0;  /* fallback */
    for (idx_t i = 0; i <= limit; ++i) prime_flag[i] = true;
    if (limit >= 0) { prime_flag[0] = false; prime_flag[1] = false; }
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (prime_flag[i]) {
            for (idx_t j = i * i; j <= limit; j += i) prime_flag[j] = false;
        }
    }

    /* Compute binomial(C(2n-1, n-1)) modulo n via prime factorisation */
    for (idx_t p = 2; p <= limit; ++p) {
        if (!prime_flag[p]) continue;

        int e = exp_in_fact((uint64_t)(2*n - 1), (uint64_t)p)
                - exp_in_fact((uint64_t)(n - 1), (uint64_t)p)
                - exp_in_fact((uint64_t)n, (uint64_t)p);
        if (e > 0) {
            seq_t pw = mod_pow((seq_t)p, (uint64_t)e, mod);
            result = (result * pw) % mod;
        }
    }

    free(prime_flag);
    return result % mod; /* ensures correct result for n == 1 */
}
