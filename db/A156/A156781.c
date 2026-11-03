/*
 * A156781  Primes : if sums of prime number and 2 consecutive prime numbers
 *          on-left-and-on-right are also primes.
 * https://oeis.org/A156781
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Validated: partial, pass, 49 terms through n = 49 (2026-11-03)
 * Helpers:   build_primes, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS A156781.
   a(n) is the n-th prime p_i such that
     p_{i-2} + p_{i-1} + p_i  is prime  AND
     p_i       + p_{i+1} + p_{i+2} is prime.
   The implementation builds a prime table up to a dynamic limit
   and rebuilds it whenever more terms are required. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/build_primes.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

static idx_t     prime_limit   = 200000;   /* current sieve bound */
static idx_t   *prime_arr      = NULL;     /* array of primes <= prime_limit */
static size_t    prime_cnt     = 0;        /* number of primes in prime_arr */

static seq_t   *valid          = NULL;     /* sequence values found     */
static size_t    valid_cnt     = 0;        /* number of stored terms    */

static bool      initialized   = false;    /* first rebuild finished    */

/* Rebuild the prime table and recompute all valid terms. */
static void rebuild(void)
{
    /* Free previous data. */
    free(prime_arr);
    mem_free(valid);

    /* Build a fresh list of primes up to prime_limit. */
    build_primes(prime_limit, &prime_arr, &prime_cnt);

    /* Prepare a container for valid terms. */
    size_t cap = prime_cnt / 10 + 10;
    if (cap < 10) cap = 10;
    valid = mem_new(seq_t, cap);
    valid_cnt = 0;

    for (size_t i = 2; i + 2 < prime_cnt; ++i) {
        /* Sums involving the current prime.   */
        seq_t left  = (seq_t)(prime_arr[i-2] + prime_arr[i-1] + prime_arr[i]);
        seq_t right = (seq_t)(prime_arr[i] + prime_arr[i+1] + prime_arr[i+2]);

        if (is_prime_big(left) && is_prime_big(right)) {
            /* Grow the valid array if necessary. */
            if (valid_cnt >= cap) {
                cap *= 2;
                seq_t *tmp = mem_resize(seq_t, valid, cap);
                if (!tmp) exit(EXIT_FAILURE); /* abort if realloc fails */
                valid = tmp;
            }
            valid[valid_cnt++] = (seq_t)prime_arr[i];
        }
    }
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    /* Ensure enough terms are available.  Grow the sieve if needed. */
    while (!initialized || valid_cnt < (size_t)n) {
        rebuild();
        if (!initialized)
            initialized = true;      /* first rebuild finished */

        if (valid_cnt >= (size_t)n)
            break;

        /* Increase the prime limit and try again. */
        prime_limit *= 2;
    }

    return valid[(size_t)n - 1];
}
