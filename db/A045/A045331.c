/*
 * A045331  Primes congruent to {1, 2, 3} mod 6; or, -3 is a square mod p.
 * https://oeis.org/A045331
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes congruent to {1, 2, 3} (mod 6)   */
/* a(n) = the n‑th prime p such that p == 2 or p%6 == 1.   */
/* (2 and 3 are the only primes ≡ 2,3 (mod 6); all others are ≡1.) */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

static seq_t  *seq_cache   = NULL;          /* list of qualifying primes */
static size_t seq_len      = 0;             /* number of elements in seq_cache   */
static size_t seq_cap      = 0;             /* allocated capacity of seq_cache    */

static uint64_t *prime_list = NULL;        /* list of all primes found          */
static size_t   primes_len  = 0;            /* number of primes in prime_list    */
static size_t   primes_cap  = 0;            /* allocated capacity of prime_list  */

static uint64_t last_checked = 1;           /* last integer checked for primality */

/* Helper to grow an array of seq_t */
static void grow_seq_cache(size_t needed)
{
    if (needed <= seq_cap) return;
    size_t newcap = seq_cap ? seq_cap * 2 : 64;
    while (newcap < needed) newcap *= 2;
    seq_t *tmp = (seq_t *)realloc(seq_cache, newcap * sizeof(seq_t));
    if (!tmp) { abort(); }
    seq_cache = tmp;
    seq_cap   = newcap;
}

/* Helper to grow an array of uint64_t */
static void grow_prime_list(size_t needed)
{
    if (needed <= primes_cap) return;
    size_t newcap = primes_cap ? primes_cap * 2 : 64;
    while (newcap < needed) newcap *= 2;
    uint64_t *tmp = (uint64_t *)realloc(prime_list, newcap * sizeof(uint64_t));
    if (!tmp) { abort(); }
    prime_list = tmp;
    primes_cap = newcap;
}

static bool is_prime_u64(uint64_t n)
{
    if (n < 2) return false;
    for (size_t i = 0; i < primes_len; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > n) break;
        if (n % p == 0) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* Ensure at least n terms are computed */
    while (seq_len < (size_t)n) {
        /* Find next candidate prime */
        last_checked += 1;
        if (last_checked == 2 || last_checked == 3) {
            /* nothing special, just handle normally */
        } else if ((last_checked & 1) == 0) {
            /* skip even numbers except 2 */
            continue;
        }

        /* Test primality */
        if (!is_prime_u64(last_checked)) continue;

        /* Record prime */
        grow_prime_list(primes_len + 1);
        prime_list[primes_len++] = last_checked;

        /* Check if it belongs to the sequence */
        if (last_checked == 2 || last_checked == 3 ||
            (last_checked % 6 == 1)) {
            grow_seq_cache(seq_len + 1);
            seq_cache[seq_len++] = (seq_t)last_checked;
        }
    }

    /* Return the nth term (1-based) */
    return seq_cache[n - 1];
}
