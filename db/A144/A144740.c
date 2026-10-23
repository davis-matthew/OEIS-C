/*
 * A144740  Partial totient function phi(c, n) for c = 2: number of semiprimes
 *          less than and coprime to n.
 * https://oeis.org/A144740
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 504, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Partial totient function phi(c, n) for c = 2:
   number of semiprimes < n that are coprime to n. */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/gcd.h"
#include "../utils/build_primes_upto.h"   /* for build_primes_upto() */

static uint64_t *s_primes = NULL;   /* array of primes up to current limit */
static size_t    s_prime_cnt = 0;   /* number of primes in s_primes */
static idx_t     s_limit = 0;       /* maximum prime limit currently built */

/* Build/extend primes up to `limit` (inclusive).  Free old array if necessary. */
static void
ensure_primes(idx_t limit)
{
    if (limit <= s_limit)
        return;
    if (s_primes != NULL) {
        free(s_primes);
        s_primes = NULL;
    }
    build_primes_upto(limit, &s_primes, &s_prime_cnt);
    s_limit = limit;
}

seq_t
a(idx_t n)
{
    if (n <= 2) /* a(1)=a(2)=0 per sequence definition */
        return (seq_t)0;

    idx_t limit = n - 1;
    ensure_primes(limit);

    seq_t count = 0;
    seq_t n_seq = (seq_t)n;

    for (size_t i = 0; i < s_prime_cnt; ++i) {
        uint64_t p = s_primes[i];
        for (size_t j = i; j < s_prime_cnt; ++j) {
            uint64_t q = s_primes[j];
            __int128 prod = (__int128)p * q;
            if (prod >= n_seq)          /* product not smaller than n */
                break;
            seq_t semiprime = (seq_t)prod;
            if (gcd(n_seq, semiprime) == 1)
                ++count;
        }
    }
    return count;
}
