/*
 * A191992  Concatenation of n and the n-th composite number.
 * https://oeis.org/A191992
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concat(n, c(n)) where c(n) is the n‑th composite number
 *            (non‑prime integer >1)
 * Range:     u8 n <= 4, u16 n <= 73, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The code builds the list of composite numbers incrementally,
   using a simple trial division primality test.  It keeps a static
   array of found composites so that repeated calls are fast.  The
   concatenation is performed by multiplying the index by 10^(digits
   of the composite) and adding the composite. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/digit_len.h"
#include "../utils/pow10.h"

/* --- static cache for composites ----------------------------------- */
static uint64_t *composites     = NULL;   /* store composite integers */
static size_t   composites_cap  = 0;     /* capacity of array            */
static size_t   composites_len  = 0;     /* number of composites stored  */
static uint64_t comp_next       = 4;     /* next integer to test        */

/* simple primality test by trial division */
static bool is_prime_u64(uint64_t n)
{
    if (n < 2) return false;
    for (uint64_t d = 2; d <= n / d; ++d)
        if (n % d == 0) return false;
    return true;
}

/* extend the cached list until it contains at least n composites */
static void extend_composites(idx_t n)
{
    while ((idx_t)composites_len < n)
    {
        /* skip primes; store composites */
        if (!is_prime_u64(comp_next))
        {
            if (composites_len == composites_cap)
            {
                composites_cap = composites_cap ? composites_cap * 2 : 16;
                composites = (uint64_t *)realloc(composites,
                                   composites_cap * sizeof(uint64_t));
            }
            composites[composites_len++] = comp_next;
        }
        ++comp_next;
    }
}

/* --- sequence function ------------------------------------------------ */
seq_t a(idx_t n)
{
    /* ensure the n‑th composite number is available */
    extend_composites(n);

    uint64_t comp      = composites[n - 1];     /* n‑th composite      */
    seq_t   comp_seq   = (seq_t)comp;           /* cast to seq_t        */
    seq_t   n_seq      = (seq_t)n;              /* unsigned index      */
    seq_t   pow10_val  = pow10((idx_t)digit_len(comp_seq));

    return n_seq * pow10_val + comp_seq;        /* concatenation       */
}
