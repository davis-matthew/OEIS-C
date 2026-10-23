/*
 * A119455  Start with 1027 and repeatedly reverse the digits and add 16 to
 *          get the next term.
 * https://oeis.org/A119455
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   reverse_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1027; a(n) = rev_decimal(a(n-1)) + 16 for n > 1
   (rev_decimal reverses the decimal digits of its argument). */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/reverse_decimal.h"

/* Memoisation of already computed terms.  The array is indexed
   starting at 1; memo[0] is unused. */
static seq_t *memo = NULL;
static idx_t memo_cap = 0;   /* capacity of the memo array (>= 1) */
static idx_t last_filled = 0;/* largest index that has a defined value */

static void grow_cache(idx_t needed)
{
    if (needed <= memo_cap)
        return;

    idx_t new_cap = memo_cap ? memo_cap << 1 : 64;
    while (new_cap < needed)
        new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(memo, sizeof(seq_t) * (new_cap + 1));
    if (!tmp) /* allocation failure – let the caller handle it */
        return;

    memo = tmp;
    memo_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                 /* undefined for n < 1, return 0 */

    grow_cache(n);

    /* compute missing terms up to n */
    for (idx_t i = last_filled + 1; i <= n; ++i) {
        if (i == 1) {
            memo[1] = (seq_t)1027;
        } else {
            memo[i] = reverse_decimal(memo[i - 1]) + (seq_t)16;
        }
    }
    last_filled = (n > last_filled) ? n : last_filled;

    return memo[n];
}
