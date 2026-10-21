/*
 * A083338  Number of partitions of odd numbers into three primes and of even
 *          numbers into two primes.
 * https://oeis.org/A083338
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 376, u16 n <= 1001, u32 n <= 1001, u64 n <= 1001,
 *            u128 n <= 1001, big any n
 * Helpers:   build_primes, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS formula:
    a(n) = number of unordered partitions of n into two primes if n is even,
           number of unordered partitions of n into three primes if n is odd.
    (Goldbach‐type count.) */

#include <stdbool.h>
#include <stdlib.h>

/*  Helpers supplied by the build system.  */
#include "../utils/build_primes.h"   /* build_primes(idx_t limit, idx_t **out, size_t *cnt); */
#include "../utils/is_prime.h"       /* bool is_prime(idx_t x); */

static idx_t *prime_list = NULL;        /* cached list of primes up to prime_limit */
static size_t  prime_cnt  = 0;          /* number of primes in prime_list            */
static idx_t  prime_limit = 0;          /* largest n processed so far                 */

seq_t a(idx_t n)
{
    /* Ensure that primes up to n (for the is_prime test) are available. */
    if (n > prime_limit) {
        if (prime_list != NULL)
            free(prime_list);
        build_primes(n, &prime_list, &prime_cnt);
        prime_limit = n;
    }

    seq_t res = 0;                      /* accumulator for the count */

    if ((n & 1) == 0) { /* even n – Goldbach partitions into two primes */
        idx_t half = n >> 1;
        for (size_t i = 0; i < prime_cnt; ++i) {
            idx_t p = prime_list[i];
            if (p > half)
                break;              /* further p will also be > half */
            idx_t q = n - p;
            if (q >= p && is_prime(q))
                ++res;
        }
    } else {                 /* odd n – partitions into three primes */
        for (size_t i = 0; i < prime_cnt; ++i) {
            idx_t p = prime_list[i];
            if (p > n)
                break;
            for (size_t j = i; j < prime_cnt; ++j) {
                idx_t q = prime_list[j];
                if (p + q > n)
                    break;
                idx_t r = n - p - q;
                if (r < q)          /* enforce unorderedness: r >= q */
                    break;
                if (is_prime(r))
                    ++res;
            }
        }
    }

    return res;
}
