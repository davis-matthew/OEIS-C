/*
 * A123122  Numbers of the form 5i + 7j for some nonnegative integers i and j.
 * https://oeis.org/A123122
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A123122 – Numbers of the form 5i+7j (i,j ≥ 0)
   Offset: 1  → a(1)=0
   Formula: a(n) = n + 11  for n > 12
   The first 12 terms are generated algorithmically. */
#include <stdbool.h>

static seq_t first[12];          /* first 12 values of the sequence   */
static bool initialized = false; /* flag for one-time initialization */

/* Check whether x can be written as 5*i + 7*j (i,j ≥ 0). */
static bool is_representable(seq_t x)
{
    /* Iterate over j.  j*7 must not exceed x. */
    for (idx_t j = 0; (seq_t)j * 7 <= x; ++j) {
        seq_t rem = x - (seq_t)j * 7;  /* x - 7*j */
        if (rem % 5 == 0) {            /* rem must be a multiple of 5 */
            return true;
        }
    }
    return false;
}

/* Compute the first 12 values of the sequence and store them in `first`. */
static void init_first(void)
{
    idx_t count = 0;
    seq_t x = 0;
    while (count < 12) {
        if (is_representable(x)) {
            first[count++] = x;
        }
        ++x;   /* consider the next integer */
    }
    initialized = true;
}

seq_t a(idx_t n)
{
    if (!initialized) {
        init_first();
    }

    if (n <= 12) {
        return first[n - 1];
    }

    /* For n > 12 the formula a(n) = n + 11 holds. */
    return (seq_t)n + 11;
}
