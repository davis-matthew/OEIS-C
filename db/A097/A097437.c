/*
 * A097437  Value of C in y = x^2 + 9x + C such that y is prime for all x = 0
 *          to 5.
 * https://oeis.org/A097437
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 20, u32 n <= 21, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A097437: find the smallest integer C such that
   x² + 9x + C is prime for all integer x = 0,1,2,3,4,5.
   The function generates terms by a memoised sequential
   search.  The j‑th term a(j) is the j‑th integer that satisfies
   the condition (a(1)=31).  It works for any seq_t/idx_t
   combination supported by the build. */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"

/* Increment amounts for x = 0..5: C, C+10, C+22, C+36, C+52, C+70 */
static const idx_t inc[6] __attribute__((unused)) = {0, 10, 22, 36, 52, 70};

/* Pre‑allocate a limit that comfortably covers all needed
   terms for the typical tests (10000 terms).  Adjust if
   necessary for a stricter memory model. */
#define MAX_CACHE 20000

static seq_t cache[MAX_CACHE];
static idx_t cache_size = 0;          /* number of values stored         */
static seq_t next_candidate = 2;      /* next integer to test           */

/* Test whether C satisfies the condition */
static bool good_candidate(seq_t C)
{
    for (idx_t i = 0; i < 6; ++i)
        if (!is_prime_big(C + (seq_t)inc[i]))
            return false;
    return true;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0; /* undefined for n < 1, but requested on OEIS   */

    /* Generate terms until we have at least n of them. */
    while (cache_size < n) {
        /* Skip non‑candidates until a good one is found. */
        while (!good_candidate(next_candidate))
            ++next_candidate;
        /* Store the found value. */
        if (cache_size < MAX_CACHE)
            cache[cache_size++] = next_candidate;
        ++next_candidate;
    }
    return cache[n - 1];
}
