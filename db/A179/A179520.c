/*
 * A179520  Primes p containing no zero digits and such that p+1 is divisible
 *          by every digit of p.
 * https://oeis.org/A179520
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 365, u32 n <= 257, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A179520: primes p containing no zero digit and such that p+1 Is divisible by every digit of p. */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

static seq_t *cached = NULL;
static size_t cached_size = 0;
static size_t cached_cap = 0;
static seq_t next_candidate = 3;  /* start with the first odd candidate */

static bool good_prime(seq_t p)
{
    seq_t plus = p + (seq_t)1;   /* p+1 */
    seq_t t = p;
    while (t) {
        seq_t d = t % (seq_t)10;
        if (d == 0) return false;
        if (plus % d != 0) return false;
        t /= (seq_t)10;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined; offset is 1 */

    if (!cached) {        /* initialise cache */
        cached_cap = 16;
        cached = (seq_t*)malloc(cached_cap * sizeof(seq_t));
        cached_size = 0;
    }

    /* generate new terms until we have at least n terms */
    while (cached_size < (size_t)n) {
        seq_t cand = next_candidate;
        next_candidate += (seq_t)2;  /* jump to next odd */

        if (!is_prime_big(cand))
            continue;

        if (!good_prime(cand))
            continue;

        /* store the qualifying prime */
        if (cached_size >= cached_cap) {
            cached_cap *= 2;
            cached = (seq_t*)realloc(cached, cached_cap * sizeof(seq_t));
        }
        cached[cached_size++] = cand;
    }

    return cached[n - 1];
}
