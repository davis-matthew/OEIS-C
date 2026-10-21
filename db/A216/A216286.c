/*
 * A216286  Primes which cannot be written as x^2+5*y^2, where x > 0, y > 0.
 * https://oeis.org/A216286
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th prime p such that p % 20 != 1 && p % 20 != 9
 *            (i.e. the n-th prime that cannot be expressed as x² + 5·y² with
 *            x>0, y>0)
 * Range:     u8 n <= 44, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   add_to_seq, add_to_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/add_to_seq.h"
#include "../utils/add_to_uint64.h"

/* primality test using existing list of primes (trial division) */
static bool is_prime_uint64(uint64_t n, uint64_t *primes, size_t prime_cnt)
{
    if (n < 2)                 return false;
    if (n == 2 || n == 3)      return true;
    if ((n & 1) == 0)          return false;          /* even numbers > 2 are composite */

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        if (p > n / p) break;               /* p*p > n */
        if (n % p == 0) return false;
    }
    return true;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* not used in the specification; defensive */

    /* static state for memoisation */
    static uint64_t *prime_list   = NULL;   /* list of primes found so far */
    static size_t   prime_cnt     = 0;
    static size_t   prime_cap     = 0;
    static uint64_t next_candidate = 2;

    static seq_t *seq   = NULL;         /* sequence values */
    static size_t seq_cnt = 0;
    static size_t seq_cap = 0;

    /* generate primes until we have enough sequence entries */
    while (seq_cnt < (size_t)n) {
        uint64_t candidate = next_candidate;
        next_candidate++;

        if (is_prime_uint64(candidate, prime_list, prime_cnt)) {
            /* store prime for future tests */
            add_to_uint64(&prime_list, &prime_cnt, &prime_cap, candidate);

            uint64_t r = candidate % 20;
            if (r != 1 && r != 9) {          /* include 5 automatically */
                add_to_seq(&seq, &seq_cnt, &seq_cap, (seq_t)candidate);
            }
        }
    }

    return seq[n-1];
}
