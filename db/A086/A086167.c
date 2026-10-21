/*
 * A086167  a(n) = sum of the first n lower twin primes.
 * https://oeis.org/A086167
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of the first n lower twin primes. A lower twin prime
 *            is the smaller prime in a twin‑prime pair (p, p+2)
 * Range:     u8 n <= 8, u16 n <= 68, u32 n <= 8730, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation keeps a growing sieve of integers and
   stores the cumulative sums of found lower twin primes.
   The sieve is recomputed from scratch whenever we need a new upper
   bound that is larger than what has previously been used.  The
   algorithm is O(L log log L) where L is the current sieve limit,
   which is comfortably fast for n ≈ 10 000 (the limit required is
   around 2 000 000). */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

/* ---------- static storage ------------------------------------------ */

static uint32_t sieve_limit = 0;            /* current sieve upper bound */
static uint8_t  *is_composite = NULL;      /* 0 = potential prime, 1 = composite */

static uint64_t *lower_twin = NULL;        /* array of lower twin primes */
static seq_t   *cumulative  = NULL;        /* cumulative sums of the above */
static size_t   twin_count = 0;            /* number of lower twin primes stored */
static size_t   twin_cap   = 0;            /* current capacity of the arrays */

/* ---------- helper functions ---------------------------------------- */

/* Re-allocate the prime sieve array to cover at least [0, new_limit]
   and initialize the sieve (composite flags).  The function
   frees the previous array if one existed. */
static void
ensure_sieve_upto (idx_t new_limit)
{
    if (new_limit <= sieve_limit)
        return;                      /* already enough */

    /* free old array */
    if (is_composite) {
        free(is_composite);
        is_composite = NULL;
    }

    /* allocate new flag array */
    is_composite = (uint8_t *)calloc(new_limit + 1, 1);   /* 0 init -> prime */
    if (!is_composite)
        return;                     /* out of memory – leave earlier state */

    /* simple Eratosthenes sieve */
    for (uint32_t i = 2; (uint64_t)i * i <= new_limit; ++i)
        if (!is_composite[i])
            for (uint64_t j = (uint64_t)i * i; j <= new_limit; j += i)
                is_composite[j] = 1;

    sieve_limit = (uint32_t)new_limit;
}

/* Recompute the list of lower twin primes until we have at least
   needed_count of them (n).  The recomputation is performed from scratch
   with a sieve bound that is repeatedly doubled until enough
   twin primes are found. */
static void
ensure_twin_primes (idx_t needed_count)
{
    if (twin_count >= needed_count)
        return;

    /* determine an upper bound large enough for needed_count twin primes */
    idx_t limit = (sieve_limit > 0) ? sieve_limit : 100;
    if (limit < 10) limit = 10;

    for (;;) {
        ensure_sieve_upto (limit);

        /* Make sure storage is sufficient */
        if (needed_count > twin_cap) {
            size_t new_cap = needed_count * 2;          /* over‑allocate a bit */
            lower_twin = (uint64_t *)realloc (lower_twin, new_cap * sizeof(uint64_t));
            cumulative  = (seq_t   *)realloc (cumulative,  new_cap * sizeof(seq_t));
            twin_cap = new_cap;
        }

        /* Rebuild the twin list and cumulative sums anew */
        twin_count = 0;
        seq_t sum = 0;

        for (uint32_t p = 3; p + 2 <= limit; p += 2) {   /* odd numbers only */
            if (!is_composite[p] && !is_composite[p + 2]) {
                lower_twin[twin_count] = p;
                sum += (seq_t)p;
                cumulative[twin_count] = sum;
                ++twin_count;
                if (twin_count == needed_count)
                    break;
            }
        }

        if (twin_count >= needed_count)
            return;      /* success */

        /* not enough twin primes – double the limit and retry */
        limit *= 2;
    }
}

/* ---------- public function ----------------------------------------- */

seq_t a (idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* offset is 1 – value for 0 not used */

    ensure_twin_primes (n);
    return cumulative[n - 1];
}
