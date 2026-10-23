/*
 * A101408  Primes p such that p+1000 or p-1000 are also primes.
 * https://oeis.org/A101408
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A101408: primes p such that p+1000 or p-1000 are also primes.
   Implementation uses a static array of up to 10000 terms, which is
   sufficient for the required range (n up to about 10000). */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"

/* Static storage for the generated terms */
static seq_t g_seq[10000];
static idx_t g_seq_len = 0;             /* number of terms currently stored */
static uint64_t g_candidate = 3;        /* next odd number to test */

/* Return the n-th term of A101408 (1‑based index). */
seq_t a(idx_t n)
{
    /* Offset is 1; behaviour for n == 0 is undefined, return 0. */
    if (n == 0) return 0;

    /* Grow the cached sequence until it contains the requested term. */
    while (g_seq_len < n)
    {
        /* Find the next prime number. */
        while (!is_prime(g_candidate))
            g_candidate += 2;          /* stay odd */

        seq_t p = (seq_t)g_candidate;

        /* Check whether p ± 1000 is also prime. */
        bool ok = false;
        if (p > 1000 && is_prime((uint64_t)(p - 1000)))
            ok = true;
        if (is_prime((uint64_t)(p + 1000)))
            ok = true;

        /* Store p if it satisfies the condition. */
        if (ok)
        {
            if (g_seq_len < 10000)
                g_seq[g_seq_len++] = p;
            else
                /* Exceeded preallocated capacity – stop generating. */
                break;
        }

        /* Prepare the next candidate (skip even numbers). */
        g_candidate += 2;
    }

    /* Return the requested term (1‑based). */
    return g_seq[n - 1];
}
