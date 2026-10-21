/*
 * A215428  Primes congruent to {0, 2, 4, 5, 6} mod 13.
 * https://oeis.org/A215428
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n‑th prime congruent to 0, 2, 4, 5 or 6 modulo 13 */
/* (OEIS A215428, offset 1).  It works for any seq_t that can hold the result. */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

static uint64_t *primes = NULL;               /* cache of qualifying primes   */
static size_t   primes_cnt = 0;               /* number of cached primes     */
static size_t   primes_cap = 0;               /* capacity of the cache       */
static uint64_t last_checked = 1;             /* last number examined         */

/* Ensure that at least `need` primes meeting the residue condition
 * are stored in `primes`.  The array grows as required. */
static void
ensure_primes(idx_t need)
{
    if (need <= (idx_t)primes_cnt)
        return;

    if (!primes) {                           /* first allocation          */
        primes_cap = 64;
        primes     = (uint64_t *)calloc(primes_cap, sizeof(uint64_t));
        if (!primes)
            abort();                          /* out of memory             */
    }

    for (;;) {
        last_checked += 1;
        uint64_t r = last_checked % 13;
        if (r == 0 || r == 2 || r == 4 || r == 5 || r == 6) {
            if (is_prime(last_checked)) {       /* check primality            */
                if (primes_cnt == primes_cap) {
                    primes_cap <<= 1;
                    uint64_t *tmp = (uint64_t *)realloc(primes, primes_cap * sizeof(uint64_t));
                    if (!tmp)
                        abort();                  /* out of memory             */
                    primes = tmp;
                }
                primes[primes_cnt++] = last_checked;
                if (primes_cnt >= (size_t)need)
                    break;
            }
        }
    }
}

seq_t
a(idx_t n)
{
    if (n == 0)
        return 0;                               /* undefined for offset < 1  */
    ensure_primes(n);
    return (seq_t)primes[(size_t)(n - 1)];
}
