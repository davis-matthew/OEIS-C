/*
 * A141698  Arrange the distinct prime divisors of each non-prime-power
 *          positive integer n in order from smallest to largest. n is
 *          included in sequence A141698 if the differences between
 *          consecutive primes in the arrangement of primes are all powers of
 *          2 (including possibly 1).
 * https://oeis.org/A141698
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) by scanning the positive integers in increasing order,
   selecting those that are not a prime power and whose distinct prime
   factors (sorted from smallest to largest) have successive gaps that
   are powers of two.  The function returns the n‑th member of that
   set. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Helper: dynamic list of primes, created on demand up to a limit.  */
static uint64_t *prime_list = NULL;      /* dynamic array of primes   */
static size_t prime_count = 0;           /* number of primes stored   */
static uint64_t prime_list_max = 1;      /* largest number checked   */
static size_t prime_list_cap = 0;        /* current allocation size  */

/* Grow the prime list capacity if necessary.  */
static void grow_prime_list(size_t needed) {
    if (prime_list_cap >= needed) return;
    size_t new_cap = prime_list_cap ? prime_list_cap * 2 : 64;
    while (new_cap < needed) new_cap *= 2;
    uint64_t *tmp = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
    if (tmp == NULL) exit(1); /* irrecoverable allocation failure */
    prime_list = tmp;
    prime_list_cap = new_cap;
}

/* Ensure that prime_list contains all prime numbers up to 'limit'.  */
static void ensure_primes(uint64_t limit) {
    if (prime_count == 0) {          /* first call: create storage   */
        grow_prime_list(64);
        prime_list[0] = 2;
        prime_count = 1;
        prime_list_max = 2;
    }
    while (prime_list_max < limit) {
        ++prime_list_max;
        bool is_prime = true;
        for (size_t i = 0; i < prime_count; ++i) {
            uint64_t p = prime_list[i];
            if (prime_list_max / p < p) break;   /* p > sqrt(max)   */
            if (prime_list_max % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            grow_prime_list(prime_count + 1);
            prime_list[prime_count++] = prime_list_max;
        }
    }
}

/* Return true if x is a power of two (x > 0).  */
static bool is_power_of_two_uint64(uint64_t x) {
    return x != 0 && (x & (x - 1)) == 0;
}

/* Return the n-th term of sequence A141698.  */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;            /* undefined, safety */
    uint64_t target = (uint64_t)n;
    uint64_t counted = 0;
    uint64_t candidate = 2;                 /* start scanning from 2 */

    /* temporary buffer for distinct prime factors (max 64 for 64‑bit). */
    uint64_t factors[64];
    size_t factor_cnt;

    while (1) {
        /* Skip prime powers: first need at least two distinct primes. */
        /* Ensure we have primes up to sqrt(candidate). */
        uint64_t sqrt_limit = 0;
        uint64_t temp = candidate;
        while (temp > 1) {                  /* rough upper bound */
            sqrt_limit = candidate;
            break;
        }
        /* Use quick test: if candidate is a perfect power of a single prime,
           we can skip early. But we'll factor anyway.                         */
        ensure_primes((uint64_t)( (candidate>>1)+1 ));

        temp = candidate;
        factor_cnt = 0;
        for (size_t i = 0; i < prime_count && prime_list[i] * prime_list[i] <= temp; ++i) {
            uint64_t p = prime_list[i];
            if (temp % p == 0) {
                factors[factor_cnt++] = p;
                while (temp % p == 0) temp /= p;
                if (factor_cnt >= 64) break;   /* safety */
            }
        }
        if (temp > 1) {
            factors[factor_cnt++] = temp;
        }

        if (factor_cnt >= 2) {               /* not a prime power */
            bool ok = true;
            for (size_t i = 1; i < factor_cnt; ++i) {
                uint64_t diff = factors[i] - factors[i - 1];
                if (!is_power_of_two_uint64(diff)) {
                    ok = false;
                    break;
                }
            }
            if (ok) {
                ++counted;
                if (counted == target) {
                    return (seq_t)candidate;
                }
            }
        }
        ++candidate;
    }
}
