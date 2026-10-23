/*
 * A139819  Complement of repdigit numbers A010785.
 * https://oeis.org/A139819
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 235, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the nth positive integer that is NOT a repdigit
   (a repdigit is a number whose decimal representation consists of
   the same digit repeated, e.g. 1, 22, 333).  The function
   computes the term by linearly scanning integers and counting
   those that are not repdigits.  This works in O(n) time; for
   n up to 10 000 the routine is easily under 2 s. */
#include <stdbool.h>

/* Helper: return true if x is a repdigit (all decimal digits equal). */
static bool is_repdigit(seq_t x)
{
    /* All one‑digit numbers are repdigits */
    if (x < 10)
        return true;

    seq_t d = x % 10;      /* last (least significant) digit */
    seq_t t = x / 10;
    while (t > 0) {
        if (t % 10 != d)
            return false;  /* a differing digit was found */
        t /= 10;
    }
    return true;           /* all digits matched */
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    idx_t count = 0;   /* how many non‑repdigits seen so far */
    seq_t candidate = 0;

    while (count < n) {
        candidate++;                     /* check next integer */
        if (!is_repdigit(candidate))
            count++;                     /* it contributes to the sequence */
    }
    return candidate;
}
