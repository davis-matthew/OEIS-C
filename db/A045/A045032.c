/*
 * A045032  Numbers whose base-4 representation contains exactly three 0's and
 *          four 1's.
 * https://oeis.org/A045032
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 370, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers whose base-4 representation contains exactly three 0's and four 1's.
   Implementation: Enumerate numbers in increasing order via lexicographic DFS.
   For each length L >= 7, generate all base‑4 strings of length L that satisfy the
   digit counts, skipping any string whose most significant digit is 0.
   The DFS explores digits in ascending order (0,1,2,3), ensuring numerical order.
   The search stops once the nth valid number is found. */

#include <stdbool.h>

static void dfs(int L, int pos, int zeros, int ones, seq_t val,
                idx_t *counter, idx_t target, seq_t *result, bool *found)
{
    if (*found) return;

    int remaining = L - pos;
    if (zeros > remaining || ones > remaining) return;
    if (pos == L) {
        if (zeros == 0 && ones == 0) {
            (*counter)++;
            if (*counter == target) {
                *result = val;
                *found = true;
            }
        }
        return;
    }

    /* Try digit 0 (if not leading) */
    if (zeros > 0 && pos != 0) {
        dfs(L, pos + 1, zeros - 1, ones, val * 4,
            counter, target, result, found);
        if (*found) return;
    }
    /* Try digit 1 */
    if (ones > 0) {
        dfs(L, pos + 1, zeros, ones - 1, val * 4 + 1,
            counter, target, result, found);
        if (*found) return;
    }
    /* Try digit 2 */
    dfs(L, pos + 1, zeros, ones, val * 4 + 2,
        counter, target, result, found);
    if (*found) return;
    /* Try digit 3 */
    dfs(L, pos + 1, zeros, ones, val * 4 + 3,
        counter, target, result, found);
}

seq_t a(idx_t n)
{
    idx_t counter = 0;
    bool found = false;
    seq_t result = 0;

    for (int L = 7; !found; ++L) {
        dfs(L, 0, 3, 4, 0,
            &counter, n, &result, &found);
    }

    return result;
}
