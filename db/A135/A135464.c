/*
 * A135464  Numbers n with property that for each single digit d of the base 3
 *          expansion of n, we can also see the base 3 expansion of d^2 as a
 *          substring. Also n may not contain any 0 digits.
 * https://oeis.org/A135464
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 32, u32 n <= 300, u64 n <= 300,
 *            u128 n <= 300, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A135464: numbers n with base‑3 digits only 1 and 2, and if a 2
 * occurs the substring “11” also occurs.  The numbers are output as the
 * base‑3 strings written in decimal notation (e.g. base‑3 “11” → 11).
 *
 * This implementation enumerates all 1‑2 strings in increasing length
 * order, checks the rule, counts them until the n‑th term is reached
 * and then returns its value.  No dynamic allocation is required so
 * it works for both integral and arbitrary‑precision build target
 * types.
 */

#include <stdbool.h>          /* for bool              */

/* internal state for the recursive search                 */
static idx_t  target_index;     /* the requested n       */
static idx_t  counter;          /* number of valid strings found so far */
static bool   found;            /* true once target found      */
static seq_t  target_value;     /* value of the n‑th term      */

/* --------------------------------------------------------------------
 * Recursive generation of all strings of length 'len'.
 * 'pos'   : current digit position (0‑based)
 * 'val'   : decimal value of the digits generated so far
 * 'seen2' : true if a '2' has already been placed
 * 'seen11': true if the substring "11" has already been seen
 * 'prev'  : previous digit placed (0 if none)
 * -------------------------------------------------------------------- */
static void generate_len(int len, int pos, seq_t val,
                         bool seen2, bool seen11, int prev)
{
    if (found) return;          /* stop early if we already found the term */

    if (pos == len) {
        if (!seen2 || seen11) { /* valid string */
            counter++;
            if (counter == target_index) {
                target_value = val;
                found = true;
            }
        }
        return;
    }

    /* Place a '1' */
    generate_len(len, pos + 1,
                 val * 10 + 1,
                 seen2, seen11 || (prev == 1), 1);

    /* Place a '2' */
    generate_len(len, pos + 1,
                 val * 10 + 2,
                 true, seen11, 2);
}

/* --------------------------------------------------------------------
 * Main function requested by the problem statement.
 * -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;       /* undefined for n < 1, return 0 for safety */

    target_index = n;
    counter = 0;
    found = false;
    target_value = 0;

    /* iterate over increasing lengths until the n‑th term is found */
    for (int len = 1; !found; ++len) {
        generate_len(len, 0, 0, false, false, 0);
    }

    return target_value;
}
