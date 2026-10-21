/*
 * A228011  The largest n-digit number whose last k digits are divisible by
 *          k^2 for k = 1..n, otherwise 0.
 * https://oeis.org/A228011
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   prefix_ok, seq_cmp, seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 *  OEIS A228011
 *  = The largest n‑digit number whose last k digits are divisible by k^2 
 *    for all k = 1..n, otherwise 0.
 *
 *  The implementation constructs the number digit by digit from the
 *  least significant side using a depth‑first search.  
 *  For each position pos (0 = least significant) the condition:
 *      (b_{pos+1} mod (pos+1)^2) == 0
 *  where b_{pos+1} is the value of the first pos+1 digits,
 *  has to hold.  The search explores digits 9..0 at every step,
 *  keeping the largest valid number found.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_from_str.h"
#include "../utils/seq_cmp.h"
#include "../utils/prefix_ok.h"

/* forward declaration of recursive search */
static void dfs(idx_t pos, idx_t n, seq_t *best, bool *best_set, uint8_t *digits);

/* Recursive depth‑first search */
static void dfs(idx_t pos, idx_t n, seq_t *best, bool *best_set, uint8_t *digits)
{
    if (pos == n) {                                 /* full number constructed */
        if (digits[n - 1] == 0) return;              /* leading digit must be non‑zero */
        /* convert digits to string in normal order */
        char *buf = (char *)malloc((size_t)n + 1);
        if (!buf) return;                           /* unreachable in normal running */
        for (idx_t i = 0; i < n; ++i)
            buf[i] = (char)('0' + digits[n - 1 - i]);
        buf[n] = '\0';
        seq_t val = seq_from_str(buf);
        free(buf);
        if (!(*best_set) || seq_cmp(&val, best) > 0) {
            *best_set = true;
            *best = val;                             /* store the best candidate */
        }
        return;
    }

    for (int d = 9; d >= 0; --d) {
        if (pos == n - 1 && d == 0) continue;        /* most significant digit can't be 0 */
        digits[pos] = (uint8_t)d;
        if (prefix_ok(pos, n, digits))
            dfs(pos + 1, n, best, best_set, digits);
    }
}

/* Public function required by the build */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                       /* nothing to compute for n=0 */

    uint8_t *digits = (uint8_t *)malloc((size_t)n * sizeof(uint8_t));
    if (!digits) return (seq_t)0;                      /* allocation failure – unlikely */

    seq_t best = (seq_t)0;
    bool best_set = false;
    dfs(0, n, &best, &best_set, digits);

    free(digits);
    return best_set ? best : (seq_t)0;
}
