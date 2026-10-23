/*
 * A108977  Numbers n such that 19*n + 17 is prime.
 * https://oeis.org/A108977
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers n such that 19*n + 17 is prime.
   a(1) = 0, a(2) = 6, etc.  This implementation pre‑allocates a
   sufficiently large static array and expands as needed.  It works for
   both fixed‑width unsigned integer types and for an arbitrary‑precision
   integer class (the same code is valid C11 and C++17). */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/is_prime.h"

/* Pre‑allocate space for up to 200 000 terms.  The tests in this
   repository never require more, and this keeps the implementation
   simple and fast in both C and C++. */
#define MAX_TERMS 200000

static seq_t terms[MAX_TERMS];
static idx_t terms_len = 0;        /* number of terms currently stored   */
static idx_t last_checked = 0;     /* n whose 19*n+17 has been tested */

/* Returns the n-th term (n ≥ 1) in the sequence.  The function may be
   called in any order; it generates terms on demand and caches them.
*/
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* offset is 1, but guard against bad caller */

    /* Generate terms until we have at least n of them. */
    while (terms_len < n)
    {
        if (terms_len >= MAX_TERMS)
        {
            /* The pre‑allocated array is too small; this should never
               happen for the intended use.  In the unlikely event it
               does, abort to avoid undefined behaviour. */
            abort();
        }

        /* Test whether 19*last_checked + 17 is prime. */
        uint64_t candidate = 19ULL * (uint64_t)last_checked + 17ULL;
        if (is_prime(candidate))
        {
            /* Store the n that satisfies the property. */
            terms[terms_len] = (seq_t)last_checked;
            terms_len++;
        }
        last_checked++;
    }

    return terms[n - 1];
}
