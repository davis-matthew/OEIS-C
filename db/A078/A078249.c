/*
 * A078249  Smallest multiple of n using all the digits other than used by n,
 *          or 0 if no such number exists.
 * https://oeis.org/A078249
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   Find the smallest multiple of n that uses all digits not present
 *            in n (in decimal), or 0 if none exists
 * Range:     u8 none, u16 none, u32 n <= 23, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   digit_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/digit_counts.h"

/* Depth‑first search for the smallest permutation of S that forms a
   multiple of n.  Parameters:
   n     – divisor (idx_t)
   len   – number of digits in S
   S     – sorted array of the available digits
   mask  – bitmask of digits already used
   rem   – current remainder of the accumulated value modulo n
   val   – current accumulated value (seq_t)
   out   – write the found value here.  */
static bool dfs(idx_t n, int len, const uint8_t *S, uint16_t mask,
                idx_t rem, seq_t val, seq_t *out)
{
    if (mask == (uint16_t)((1u << len) - 1)) {
        if (rem == 0) {
            *out = val;
            return true;
        }
        return false;
    }

    for (int i = 0; i < len; ++i) {
        if (mask & (1u << i))
            continue;
        uint8_t d = S[i];
        if (val == (seq_t)0 && d == 0)   /* don't allow leading zero */
            continue;
        idx_t newRem = (rem * 10 + d) % n;
        seq_t newVal = val * (seq_t)10 + (seq_t)d;
        if (dfs(n, len, S, mask | (1u << i), newRem, newVal, out))
            return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0u)
        return (seq_t)0;

    /* Quick impossible cases: */
    if (n % 10u == 0u)                 /* ends with 0 */
        return (seq_t)0;

    unsigned char cnt[10] = {0};
    digit_counts((seq_t)n, cnt);

    bool used[10];
    for (int d = 0; d < 10; ++d)
        used[d] = cnt[d] > 0;

    /* n ends with 5 and uses 0 */
    if ((n % 10u == 5u) && used[0]) {
        return (seq_t)0;
    }

    /* n uses all even digits */
    if (used[0] && used[2] && used[4] && used[6] && used[8]) {
        return (seq_t)0;
    }

    /* n uses all nine non‑zero digits */
    bool all9 = true;
    for (int d = 1; d <= 9; ++d) {
        if (!used[d]) {
            all9 = false;
            break;
        }
    }
    if (all9)
        return (seq_t)0;

    /* Build the set of complement digits */
    uint8_t S[10];
    int len = 0;
    for (int d = 0; d < 10; ++d)
        if (!used[d])
            S[len++] = (uint8_t)d;

    if (len == 0)
        return (seq_t)0;             /* n uses all digits */

    seq_t found = (seq_t)0;
    if (dfs(n, len, S, 0, 0, (seq_t)0, &found))
        return found;

    return (seq_t)0;                 /* no valid multiple found */
}
