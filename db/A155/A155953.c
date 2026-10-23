/*
 * A155953  Emirps of the form k^2 + k + 41.
 * https://oeis.org/A155953
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 23, u32 n <= 23, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   is_prime_big, rev_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Emirps of the form k^2 + k + 41.
   The function scans k ≥ 0, computes p = k^2 + k + 41,
   keeps those primes whose decimal reverse is also prime
   (and not equal to p), and returns the n‑th such number. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/rev_decimal.h"

static seq_t *emirps    = NULL;   /* cache of found emirps */
static size_t emirps_cap = 0;
static idx_t  next_k     = 0;     /* next k to test */
static idx_t  emirps_cnt  = 0;     /* number of emirps stored */

seq_t a(idx_t n)
{
    /* Generate emirps until we have at least n of them. */
    while (emirps_cnt < n)
    {
        seq_t k  = (seq_t)next_k;
        seq_t p  = k * (k + (seq_t)1) + (seq_t)41;  /* k^2 + k + 41 */

        if (is_prime_big(p))
        {
            seq_t rev = rev_decimal(p);
            if (rev != p && is_prime_big(rev))
            {
                /* Grow cache if necessary. */
                if (emirps_cnt + 1 > emirps_cap)
                {
                    size_t new_cap = (emirps_cap == 0) ? 16 : emirps_cap * 2;
                    seq_t *tmp = (seq_t *)realloc(emirps, new_cap * sizeof(seq_t));
                    if (!tmp)
                        abort(); /* out of memory */
                    emirps = tmp;
                    emirps_cap = new_cap;
                }
                emirps[emirps_cnt++] = p;
            }
        }
        ++next_k;
    }
    return emirps[n - 1];
}
