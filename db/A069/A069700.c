/*
 * A069700  Triangular numbers with internal digits 9.
 * https://oeis.org/A069700
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 19, u32 n <= 19, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   internal_digits9
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: enumerate triangular numbers T(k)=k*(k+1)/2 for
   k ≥ 0 up to a fixed bound and keep those whose internal decimal
   digits are all 9.  The known largest term of this finite
   sequence is 5995 (k = 109).  a(1) = 0, a(2) = 1, etc.  If n is
   larger than the number of terms found, zero is returned. */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/internal_digits9.h"

#define MAX_K   20000          /* enough to capture all known terms */
#define MAX_TERMS 200          /* storage for the (rare) terms */

static seq_t terms[MAX_TERMS];
static size_t term_count = 0;
static bool initialized = false;

/* One‑time initialization: generate all qualifying triangular
   numbers up to MAX_K.  The list is stored for fast random
   access. */
static void init_terms(void)
{
    term_count = 0;
    for (idx_t k = 0; k <= MAX_K; ++k) {
        seq_t tri = (seq_t)k * ((seq_t)k + 1) / 2;
        if (internal_digits9(tri)) {
            if (term_count < MAX_TERMS)
                terms[term_count++] = tri;
        }
    }
    initialized = true;
}

seq_t a(idx_t n)
{
    if (!initialized)
        init_terms();

    if (n == 0 || n > term_count)   /* offset starts at 1 */
        return (seq_t)0;
    return terms[n - 1];
}
