/*
 * A144592  Composites of the form smallest digit of prime(n) + prime(n).
 * https://oeis.org/A144592
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   is_prime, min_digit_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A144592: a(n) = composite numbers of the form prime(n) + smallest
 *  digit of prime(n), in the order of increasing primes; duplicates are kept.
 *  The function builds the needed primes on demand and memoises the
 *  sequence so that subsequent calls are fast.  */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/min_digit_of.h"

/*  The static state that is shared between calls to a()  */
static uint64_t last_checked = 1;          /* number just checked for primality     */
static uint64_t *prime_list = NULL;        /* primes found so far                   */
static size_t prime_cap = 0;
static size_t prime_len = 0;

static seq_t *comps = NULL;                /* composites values found so far        */
static size_t comps_cap = 0;
static size_t comps_len = 0;

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset 1 – behaviour below irrelevant */

    /*  grow sequence until we have at least n terms  */
    while (comps_len < (size_t)n) {
        /*  find next prime  */
        do {
            ++last_checked;
        } while (!is_prime((idx_t)last_checked));

        /*  store the new prime  */
        if (prime_len == prime_cap) {
            prime_cap = prime_cap ? prime_cap * 2 : 4;
            uint64_t *tmp = (uint64_t*)realloc(prime_list, prime_cap * sizeof(uint64_t));
            if (!tmp) abort();            /* allocation failed – terminate */
            prime_list = tmp;
        }
        prime_list[prime_len++] = last_checked;

        /*  compute p + smallest digit of p  */
        uint64_t d = min_digit_of(last_checked);
        seq_t sum = (seq_t)last_checked + (seq_t)d;

        /*  keep only composite sums  */
        if (!is_prime((idx_t)sum)) {
            if (comps_len == comps_cap) {
                comps_cap = comps_cap ? comps_cap * 2 : 4;
                seq_t *tmp = (seq_t*)realloc(comps, comps_cap * sizeof(seq_t));
                if (!tmp) abort();
                comps = tmp;
            }
            comps[comps_len++] = sum;
        }
    }

    return comps[n - 1];
}
