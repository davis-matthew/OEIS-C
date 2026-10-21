/*
 * A214437  Least numbers whose groups of 2,3,...,n digits taken from the left
 *          are divisible by 2,3,...,n.
 * https://oeis.org/A214437
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for the least n‑digit number whose first k digits
   (1≤k≤n) are divisible by k for every k≥2.  The sequence stops
   after the 25th term; larger n return 0. */
#include <stdbool.h>

/* Helper recursive search that constructs the minimal number
   digit by digit.  cur holds the value of the current prefix. */
static bool dfs(idx_t len, idx_t target, seq_t cur, seq_t *out)
{
    if (len == target) {
        *out = cur;
        return true;
    }
    int start = (len == 0) ? 1 : 0;          /* first digit cannot be 0 */
    for (int d = start; d <= 9; ++d) {
        seq_t new_val = cur * (seq_t)10 + (seq_t)d;
        idx_t new_len = len + 1;
        if (new_len >= 2) {
            if (new_val % (seq_t)new_len != (seq_t)0)
                continue;                   /* must be divisible by new_len */
        }
        if (dfs(new_len, target, new_val, out))
            return true;                     /* minimal found */
    }
    return false;                            /* no valid continuation */
}

/* Cache of the first 25 values.  0 indicates “not computed yet” or “no solution”. */
static seq_t cache[26] = {0};
static bool cached[26] = {0};

seq_t a(idx_t n)
{
    if (n == 0 || n > 25)         /* offset is 1 → a(1) is the first term */
        return (seq_t)0;
    if (cached[n])                /* already computed */
        return cache[n];

    seq_t val = (seq_t)0;
    if (dfs((idx_t)0, n, (seq_t)0, &val))
        cache[n] = val;
    cached[n] = true;
    return cache[n];
}
