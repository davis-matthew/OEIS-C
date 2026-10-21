/*
 * A082888  Primes p such that (r-p)/log(p) > 3, where r is the next prime
 *          after p.
 * https://oeis.org/A082888
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 149, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   append_candidate, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A082888: primes p such that (next_prime(p)-p)/ln(p) > 3
 *
 * The routine generates the primes in order, tests each consecutive
 * pair, and stores p whenever the inequality holds.  It keeps a static
 * cache so repeated calls are fast, and it works for any value of the
 * supported integer types (uint8_t … unsigned __int128, or an
 * arbitrary‑precision integer class).
 *
 * The implementation uses the supplied utility `next_prime`
 * (returns the least prime > start) and `append_candidate`
 * (dynamically grows the cache array).
 */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/append_candidate.h"
#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    /* Offset is 1; behaviour for n==0 is unused */
    if (n == 0)
        return 0;

    /* static cache of qualifying primes */
    static seq_t *qual = NULL;
    static size_t qlen = 0, qcap = 0;
    /* next_prime start: the smallest prime to examine */
    static uint64_t start = 2;

    /* grow the cache until it contains at least n elements */
    while ((size_t)n > qlen)
    {
        uint64_t p = start;
        uint64_t r = next_prime(p);
        double diff   = (double)(r - p);
        double cond   = diff / log((double)p);
        if (cond > 3.0)
            append_candidate(&qual, &qlen, &qcap, (seq_t)p);
        start = r;            /* move to the next prime to test */
    }

    /* return the requested element (1‑based index) */
    return qual[(size_t)n - 1];
}
