/*
 * A225538  Let r(n) denote the reverse of n. For every n, consider the
 *          sequence n_1 = n + 1 + r(n+1), and for m >= 2, n_m = n_(m-1) + 1 +
 *          r(n_(m-1) + 1). a(n) is the least m for which n_m is a palindrome,
 *          or 0 if there is no such m.
 * https://oeis.org/A225538
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 67, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   is_palindrome, reverse_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A225538:  a(n) = the least m such that n_m is a palindrome, where
 *           n_1 = n + 1 + rev(n+1) and n_m = n_(m-1) + 1 + rev(n_(m-1)+1).
 *           Returns 0 if no palindrome is reached within the search limit.
 */

#include <stdbool.h>
#include "../utils/reverse_decimal.h"   /* seq_t reverse_decimal(seq_t) */
#include "../utils/is_palindrome.h"     /* bool is_palindrome(seq_t, idx_t) */

/* Search limit: sufficient to cover the known values for n up to ~10 000. */
static const idx_t MAX_STEPS = 2000;

/* Compute a(n) for the OEIS sequence A225538 */
seq_t a(idx_t n) {
    /* First term: n_1 = n + 1 + rev(n+1) */
    seq_t cur = ((seq_t)n) + 1 + reverse_decimal((seq_t)n + 1);

    if (is_palindrome(cur, 10))
        return (seq_t)1;

    for (idx_t step = 2; step <= MAX_STEPS; ++step) {
        cur = cur + 1 + reverse_decimal(cur + 1);
        if (is_palindrome(cur, 10))
            return (seq_t)step;
    }

    /* No palindrome found within the search limit */
    return (seq_t)0;
}
