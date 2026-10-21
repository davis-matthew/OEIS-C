/*
 * A225614  The smallest n-digit number whose first k digits are divisible by
 *          the k-th prime for k = 1..n.
 * https://oeis.org/A225614
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   a(n) is the smallest n‑digit integer whose first k digits are
 *            divisible by the k‑th prime for k = 1…n
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses a depth‑first search with backtracking
   exploring digits in ascending order.  A small static array of the
   first ten primes is used because the definition requires only up
   to ten digits (the sequence has exactly 10 terms). */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* the k‑th prime, 1‑based; we need only the first ten */
static const uint32_t primes[10] = {2,3,5,7,11,13,17,19,23,29};

/* Recursive helper: try to build the n‑digit number digit by digit.
   depth   – current length of the built prefix
   prefix  – numeric value of the prefix (as seq_t)
   target  – total number of digits required (n)
   result  – storage for the finished number once found
   Returns true when a valid number of length target is found. */
static bool dfs(idx_t depth, seq_t prefix, idx_t target, seq_t *result)
{
    if (depth == target) {           /* full length reached */
        *result = prefix;
        return true;
    }

    seq_t prime = (seq_t)primes[depth];   /* prime for position depth+1 */
    for (int d = 0; d <= 9; ++d) {
        if (depth == 0 && d == 0)        /* no leading zero */
            continue;

        seq_t new_prefix = prefix * 10 + (seq_t)d;
        if (new_prefix % prime == 0) {
            if (dfs(depth + 1, new_prefix, target, result))
                return true;            /* propagate success upward */
        }
    }
    return false;                       /* no digit leads to a solution */
}

/* Public entry point required by the framework. */
seq_t a(idx_t n)
{
    if (n == 0 || n > 10)          /* the sequence is defined only up to 10 */
        return (seq_t)0;

    seq_t result = 0;
    dfs(0, (seq_t)0, n, &result);
    return result;
}
