/*
 * A046458  Positive even integers that are not the sum of two nonprime odd
 *          integers.
 * https://oeis.org/A046458
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A046458 – Positive even integers not the sum of two nonprime odd integers. */
/* Computed by exhaustive search up to 38; beyond that all even integers are representable. */

#include <stdbool.h>
#include "../utils/is_prime.h"

/* Helper: true if x is an odd integer that is not prime (1 counts as nonprime). */
static bool is_nonprime_odd(idx_t x)
{
    if (x % 2 == 0)              /* even numbers are not considered */
        return false;
    if (x <= 1)                  /* 1 is odd and nonprime */
        return true;
    return !is_prime(x);         /* otherwise check prime status */
}

/* Return the n-th term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0)            /* undefined, return 0 as a safe fallback */
        return 0;

    idx_t found = 0;

    /* Iterate over even candidates starting at 4 up to 38 – afterwards all are representable. */
    for (idx_t candidate = 4; candidate <= 38; candidate += 2) {
        bool representable = false;

        /* Try all odd nonprime a with 1 ≤ a < candidate. */
        for (idx_t a = 1; a < candidate; a += 2) {
            if (!is_nonprime_odd(a))
                continue;
            idx_t b = candidate - a;
            if (!is_nonprime_odd(b))
                continue;
            representable = true;
            break;
        }

        if (!representable) {
            ++found;
            if (found == n)
                return (seq_t)candidate;   /* found the required term */
        }
    }

    /* If n is larger than the number of terms (currently 8), return 0. */
    return 0;
}
