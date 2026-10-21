/*
 * A078403  Primes whose digital root (A038194) is prime.
 * https://oeis.org/A078403
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   dr_is_prime, grow_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute primes whose digital root is prime (OEIS A078403) by
   generating primes incrementally and keeping those with a prime
   digital root.  The function works for any sequence type seq_t
   and index type idx_t. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/dr_is_prime.h"
#include "../utils/grow_array.h"

/* prime list (uint64_t) – expands as needed */
static uint64_t *primes      = NULL;
static size_t   primes_cnt   = 0;
static size_t   primes_cap   = 0;

/* list of primes with prime digital root */
static uint64_t *good        = NULL;
static size_t   good_cnt     = 0;
static size_t   good_cap     = 0;

/* last integer that was inspected when extending the lists */
static uint64_t last_checked = 1;

/* ---------------- helper functions ----------------------------------- */

/* primality test for uint64_t (trial division) */
static inline bool is_prime_uint64(uint64_t x)
{
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    for (uint64_t d = 3; d * d <= x; d += 2) {
        if (x % d == 0) return false;
    }
    return true;
}

/* generate primes and good primes until we have at least target_good entries */
static void fill_to(size_t target_good)
{
    while (good_cnt < target_good) {
        last_checked++;
        if (is_prime_uint64(last_checked)) {
            grow_array(&primes, &primes_cnt, &primes_cap);
            primes[primes_cnt++] = last_checked;
            if (dr_is_prime(last_checked)) {
                grow_array(&good, &good_cnt, &good_cap);
                good[good_cnt++] = last_checked;
            }
        }
    }
}

/* ------------------ main sequence function -------------------------- */

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;        /* undefined below offset 1 */

    /* if we already have the value, return it */
    if ((size_t)(n - 1) < good_cnt)
        return (seq_t)good[n - 1];

    /* otherwise generate more primes until n-th good prime is found */
    fill_to((size_t)n);
    return (seq_t)good[n - 1];
}
