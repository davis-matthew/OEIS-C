/*
 * A110025  Smallest primes starting a complete three iterations Cunningham
 *          chain of the first or second kind.
 * https://oeis.org/A110025
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 35, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A110025: smallest primes that start a complete
    three‑iteration Cunningham chain of the first or second kind.
    A prime p is accepted if

       (a)  p, 2p±1, 4p±3, 8p±7 are all prime
            (the sign is + for the first kind, – for the second);
       (b)  the next number in the chain, 16p±15, is composite;
       (c)  p is the smallest prime in its chain, i.e. there is
            no prime q with 2q+1 = p for the first kind or
            2q-1 = p for the second kind.

    The function searches primes in ascending order and memoises
    found terms so that further calls are fast. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"

static seq_t *results   = NULL;   /* stored terms in ascending order   */
static size_t res_cap = 0;        /* capacity of the array             */
static size_t res_size = 0;       /* number of terms stored            */
static uint64_t probe   = 2;      /* next integer to test for primality */

static void add_result(seq_t p)
{
    if (res_size + 1 > res_cap) {
        size_t new_cap = res_cap ? (res_cap << 1) : 64;
        seq_t *tmp = (seq_t *)realloc(results, new_cap * sizeof(seq_t));
        if (!tmp) exit(1); /* allocation failure */
        results = tmp;
        res_cap = new_cap;
    }
    results[res_size++] = p;
}

/*  A first‑kind chain: p -> 2p+1 -> 4p+3 -> 8p+7,
    with 16p+15 composite.  Also reject p if it has a
    predecessor 2q+1 with q prime. */
static bool chain_first_kind(uint64_t p)
{
    /* reject if predecessor exists */
    if (p > 2) {
        uint64_t q = (p - 1) >> 1;
        if (is_prime(q))
            return false;
    }

    uint64_t n1 = 2 * p + 1;
    if (!is_prime(n1)) return false;
    uint64_t n2 = 2 * n1 + 1;
    if (!is_prime(n2)) return false;
    uint64_t n3 = 2 * n2 + 1;
    if (!is_prime(n3)) return false;
    uint64_t n4 = 2 * n3 + 1;
    return !is_prime(n4);
}

/*  A second‑kind chain: p -> 2p-1 -> 4p-3 -> 8p-7,
    with 16p-15 composite.  Also reject p if it has a
    predecessor 2q-1 with q prime. */
static bool chain_second_kind(uint64_t p)
{
    /* reject if predecessor exists */
    if (p > 2) {
        uint64_t q = (p + 1) >> 1;
        if (is_prime(q))
            return false;
    }

    uint64_t n1 = 2 * p - 1;
    if (!is_prime(n1)) return false;
    uint64_t n2 = 2 * n1 - 1;
    if (!is_prime(n2)) return false;
    uint64_t n3 = 2 * n2 - 1;
    if (!is_prime(n3)) return false;
    uint64_t n4 = 2 * n3 - 1;
    return !is_prime(n4);
}

/*  Ensure that at least n terms have been found.  It keeps
    scanning numbers from where it left off.  */
static void ensure_size(idx_t n)
{
    if (n < 1) return;
    while (res_size < (size_t)n) {
        if (is_prime(probe)) {
            if (chain_first_kind(probe) || chain_second_kind(probe))
                add_result((seq_t)probe);
        }
        ++probe;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n < 1 */
    ensure_size(n);
    return results[(size_t)n - 1];
}
