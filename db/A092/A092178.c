/*
 * A092178  Primes congruent to 8 mod 13.
 * https://oeis.org/A092178
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 546, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A092178 – Primes congruent to 21 mod 26 (i.e., 8 mod 13).
   The function a(n) returns the n-th such prime (a(1) = 47).  It
   builds a simple sieve of Eratosthenes incrementally and extracts
   the primes in the progression 21 + 26*k. */
   
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

/*  Build and maintain a sieve of Eratosthenes up to 'limit'.
    New memory is allocated as necessary; previous data is freed.
    The function recomputes the list of primes in the AP 21+26*k
    and stores them in the global array 'ap_primes'. */
static size_t sieve_limit = 0;
static unsigned char *is_composite = NULL;
static uint32_t *ap_primes = NULL;
static size_t ap_len = 0;

static void extend_sieve(size_t new_limit)
{
    if (new_limit <= sieve_limit) return;
    /* Allocate new sieve array */
    unsigned char *new_comp = (unsigned char *)calloc(new_limit + 1, 1);
    if (!new_comp) abort();

    /* Mark 0 and 1 as composite */
    if (new_limit >= 0) new_comp[0] = 1;
    if (new_limit >= 1) new_comp[1] = 1;

    /* Even numbers >2 are composite */
    for (size_t i = 4; i <= new_limit; i += 2) new_comp[i] = 1;

    /* Sieve odd numbers */
    size_t r = (size_t)sqrt((double)new_limit);
    for (size_t i = 3; i <= r; i += 2) {
        if (!new_comp[i]) {
            size_t step = i * 2;
            size_t start = i * i;
            for (size_t j = start; j <= new_limit; j += step)
                new_comp[j] = 1;
        }
    }

    /* Recompute list of primes in the progression 21+26*k */
    size_t count = 0;
    for (size_t p = 21; p <= new_limit; p += 26)
        if (!new_comp[p]) ++count;

    uint32_t *new_ap = (uint32_t *)malloc(count * sizeof(uint32_t));
    if (!new_ap) abort();

    size_t idx = 0;
    for (size_t p = 21; p <= new_limit; p += 26)
        if (!new_comp[p]) new_ap[idx++] = (uint32_t)p;

    /* Free old data and install new data */
    free(is_composite);
    is_composite = new_comp;

    free(ap_primes);
    ap_primes = new_ap;
    ap_len = count;
    sieve_limit = new_limit;
}

/*  Return the n-th prime congruent to 21 (mod 26).  The function
    extends the sieve until the requested term is available.  It
    works for any n >= 1.  It is thread‑unsafe because of the global
    state, but the original specification assumes a single process. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* Undefined for n=0, but harmless */

    /* Initial estimate: enough so that at least n terms of the AP
       should appear.  For the typical case n <= 1e4 this gives a limit
       of a few million numbers. */
    size_t limit = (n < 128) ? 1000 : n * 26 / 12;
    while (ap_len < (size_t)n) {
        extend_sieve(limit);
        if (ap_len < (size_t)n) limit *= 2;   /* double if not enough */
    }

    return (seq_t)ap_primes[n - 1];
}
