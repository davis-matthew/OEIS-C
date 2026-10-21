/*
 * A099861  a(n) = (2*n-1)-st composite number: a bisection of A002808.
 * https://oeis.org/A099861
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 9592, u32 n <= 9592, u64 n <= 9592,
 *            u128 n <= 9592, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the (2*n-1)-st composite number, starting with 4 as the first composite. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Helper: test primality by trial division, suitable for numbers up to a few
   millions which is enough for all n requested in the tests (n≤10000).           */
static bool is_prime_uint64(uint64_t x)
{
    if (x < 2)          return false;
    if (x % 2 == 0)     return x == 2;
    if (x % 3 == 0)     return x == 3;
    for (uint64_t d = 5; d * d <= x; d += 6) {
        if (x % d == 0)     return false;
        if (x % (d + 2) == 0) return false;
    }
    return true;
}

/* Static storage for composite numbers (as 64‑bit values).          */
static uint64_t *composites = NULL;      /* dynamic array of composites   */
static size_t composites_len = 0;        /* number of composites stored   */
static size_t composites_cap = 0;        /* allocated array capacity      */
static uint64_t next_candidate = 4;      /* next integer to test          */

/* Grow the composites buffer if necessary.                       */
static void ensure_composite_buffer(void)
{
    if (composites_len == composites_cap) {
        size_t new_cap = (composites_cap == 0) ? 64 : composites_cap * 2;
        uint64_t *tmp = (uint64_t *)realloc(composites, new_cap * sizeof(uint64_t));
        if (!tmp) abort();
        composites = tmp;
        composites_cap = new_cap;
    }
}

/* Generate composites until we have at least (target_idx+1) entries. */
static void generate_composites_till(size_t target_idx)
{
    while (composites_len <= target_idx) {
        uint64_t v = next_candidate;
        /* Determine if v is composite.                         */
        bool composite = false;
        if (v > 1 && !is_prime_uint64(v))      /* not prime → composite */
            composite = true;
        /* Store if composite.                                 */
        if (composite) {
            ensure_composite_buffer();
            composites[composites_len++] = v;
        }
        next_candidate++;                     /* advance to next number   */
    }
}

/* Main sequence function -------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* undefined for offset 0   */
    /* desired index in the composites array (0‑based) */
    size_t target_idx = (size_t)(2 * n - 2);
    generate_composites_till(target_idx);
    return (seq_t)composites[target_idx];
}
