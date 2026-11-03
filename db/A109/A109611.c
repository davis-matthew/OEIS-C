/*
 * A109611  Chen primes: primes p such that p + 2 is either a prime or a
 *          semiprime.
 * https://oeis.org/A109611
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth Chen prime (prime p where p+2 is prime or semiprime).
 *            Computed by scanning integers and using is_prime and
 *            is_semiprime_uint64
 * Range:     u8 n <= 41, u16 n <= 2992, u32 n <= 34076, u64 n <= 34076,
 *            u128 n <= 34076, big any n
 * Validated: standard, pass, 73 terms through n = 9851 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

/* true if x is the product of exactly two primes (possibly the same) */
static bool is_semiprime_uint64(uint64_t x)
{
    if (x < 4)
        return false;
    uint64_t d = 0;
    if (x % 2 == 0)
        d = 2;
    else {
        for (uint64_t dd = 3; dd <= x / dd; dd += 2) {
            if (x % dd == 0) {
                d = dd;
                break;
            }
        }
    }
    if (d == 0)
        return false;                      /* x is prime */
    uint64_t second = x / d;
    return is_prime_u64(second);
}

/* Dynamic array helpers. */
static void push_seq(seq_t **arr, idx_t *cap, idx_t *size, seq_t val)
{
    if (*size >= *cap) {
        size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
        *arr = mem_resize(seq_t, *arr, new_cap);
        *cap = new_cap;
    }
    (*arr)[*size] = val;
    (*size)++;
}

static idx_t next_candidate = 2;
static idx_t cache_size = 0;
static idx_t cache_cap = 0;
static seq_t *cache = NULL;

seq_t a(idx_t n) {
    /* Return the nth Chen prime. */
    if (n < 1) return 0;
    while (cache_size < n) {
        if (is_prime_u64((uint64_t)next_candidate)) {
            uint64_t p2 = next_candidate + 2;
            if (is_prime_u64(p2) || is_semiprime_uint64(p2)) {
                push_seq(&cache, &cache_cap, &cache_size, (seq_t)next_candidate);
            }
        }
        next_candidate++;
    }
    return cache[n - 1];
}
