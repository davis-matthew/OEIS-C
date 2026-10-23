/*
 * A198327  Semiprimes k such that k-2 is also a semiprime.
 * https://oeis.org/A198327
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A198327: Semiprimes k such that k-2 is also a semiprime.
   Computed by enumerating integers and testing the property. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/build_spf.h"   /* build_spf(limit, &spf_ptr) */

static idx_t *spf = NULL;          /* spf[x] = smallest prime factor of x (0 if x < 2) */
static idx_t spf_limit = 0;        /* current maximum indexed by spf (inclusive) */

/* Ensure spf[0..limit] is valid.  The array is enlarged as needed. */
static void ensure_spf(idx_t limit)
{
    if (limit <= spf_limit)
        return;   /* already available */

    idx_t new_limit = limit * 2 + 10;
    if (new_limit <= limit)          /* guard against overflow */
        new_limit = limit + 10;

    idx_t *new_spf = NULL;
    build_spf(new_limit, &new_spf);  /* provided by utils */

    if (spf) {
        free(spf);
    }
    spf = new_spf;
    spf_limit = new_limit;
}

/* Test if x is a semiprime using the spf array. */
static bool is_semiprime(idx_t x)
{
    if (x < 4)                     /* 4 = 2*2 is the smallest semiprime */
        return false;

    idx_t p = spf[x];              /* guaranteed to exist after ensure_spf(x) */
    if (p == 0 || p == x)          /* primes or 0/1 */
        return false;

    idx_t q = x / p;
    return spf[q] == q;            /* q must also be prime */
}

/* Public function demanded by the framework:  a(idx_t n) -> seq_t           */
seq_t a(idx_t n)
{
    if (n == 0)                    /* undefined for n=0, but return 0 for safety */
        return 0;

    idx_t count = 0;
    for (idx_t x = 3; ; ++x) {
        ensure_spf(x);             /* make sure spf information up to 'x' exists */

        if (is_semiprime(x) && is_semiprime(x - 2)) {
            ++count;
            if (count == n)
                return (seq_t)x;
        }
    }
}
