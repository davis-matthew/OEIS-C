/*
 * A049596  Primes p such that x^9 = 2 has a solution mod p.
 * https://oeis.org/A049596
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = primes p such that 2 is a 9th power residue modulo p
 * Range:     u8 n <= 35, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd, isPrimeTrial, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/mod_pow.h"
#include "../utils/isPrimeTrial.h"

static seq_t *good = NULL;
static size_t good_cap = 0;
static size_t good_len = 0;
static uint64_t candidate = 3;

static void grow_good(void)
{
    size_t new_cap = (good_cap == 0) ? 64 : good_cap * 2;
    seq_t *new_ptr = (seq_t *)realloc(good, new_cap * sizeof(seq_t));
    if (!new_ptr) exit(1);          /* allocation failed */
    good = new_ptr;
    good_cap = new_cap;
}

static void ensure_good(idx_t n)
{
    while (good_len < n)
    {
        if (good_len >= good_cap) grow_good();

        if (good_len == 0)
        {
            /* first term is always 2 */
            good[good_len++] = (seq_t)2;
            continue;
        }

        /* test current candidate for primality */
        if (isPrimeTrial((seq_t)candidate))
        {
            /* candidate is prime */
            seq_t g = gcd((seq_t)9, (seq_t)(candidate - 1));
            seq_t exp = (seq_t)(candidate - 1) / g;
            if (mod_pow((seq_t)2, (uint64_t)exp, (seq_t)candidate) == (seq_t)1)
                good[good_len++] = candidate;
        }

        candidate += 2;                 /* next odd number */
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;         /* undefined for n=0, not used */
    ensure_good(n);
    return good[n - 1];
}
