/*
 * A072568  Even interprimes.
 * https://oeis.org/A072568
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 3621, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = (p_k + p_{k+1}) / 2 where p_k and p_{k+1} are
 * consecutive odd primes and the result is even.
 * This yields the even interprimes of A072568.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* offset is 1; n≥1 for valid terms */

    /* A conservative upper bound for the prime value needed.
       For n up to about 10000, 30*n + 1000 comfortably exceeds the
       2*n‑th prime. */
    idx_t limit = n * 30 + 1000;
    if (limit < 100) limit = 100;   /* minimum sieve size */

    uint64_t *primes = NULL;
    size_t prime_cnt = 0;
    build_primes_upto(limit, &primes, &prime_cnt);   /* primes <= limit */

    /* Scan consecutive odd primes and collect even midpoints */
    size_t found = 0;
    seq_t result = 0;
    for (size_t i = 0; i + 1 < prime_cnt; ++i) {
        seq_t sum = (seq_t)primes[i] + (seq_t)primes[i + 1];
        if ((sum & 3ULL) == 0) {        /* sum % 4 == 0  →  even midpoint */
            ++found;
            if (found == n) {
                result = sum >> 1;     /* divide by 2 */
                break;
            }
        }
    }

    free(primes);
    return result;
}
