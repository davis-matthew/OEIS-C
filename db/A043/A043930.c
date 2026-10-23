/*
 * A043930  Numbers k such that 4 and 6 occur juxtaposed in the base-7
 *          representation of k but not of k+1.
 * https://oeis.org/A043930
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers k such that 4 and 6 occur juxtaposed in the base‑7
 *            representation of k but not of k+1. Sequence is indexed with
 *            a(1) = 34, a(2) = 46,
 * Range:     u8 n <= 10, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* --- internal storage for memoised sequence --- */
static seq_t *seq_buf = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;
static seq_t search_curr = 1;   /* next natural number to test */

/* check whether a base‑7 representation of val contains
   a "4,6" or "6,4" adjacent pair */
static bool has_adj(const seq_t val)
{
    int prev = -1;
    seq_t v = val;
    while (v > 0) {
        int d = (int)(v % 7);
        if (prev >= 0) {
            if ((d == 4 && prev == 6) || (d == 6 && prev == 4))
                return true;
        }
        prev = d;
        v /= 7;
    }
    return false;
}

/* grow the buffer enough to hold new_len elements */
static void ensure_capacity(size_t new_len)
{
    if (seq_cap < new_len) {
        size_t new_cap = seq_cap ? seq_cap << 1 : 64;
        while (new_cap < new_len) new_cap <<= 1;
        seq_buf = (seq_t*)realloc(seq_buf, new_cap * sizeof(seq_t));
        seq_cap = new_cap;
    }
}

/* main function required by the test harness */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* undefined for n<1, return 0 */

    /* extend stored values up to n */
    while (seq_len < (size_t)n) {
        ensure_capacity((size_t)n);
        /* scan natural numbers until we get one more term */
        while (seq_len < (size_t)n) {
            if (has_adj(search_curr) && !has_adj(search_curr + 1)) {
                seq_buf[seq_len++] = search_curr;
            }
            ++search_curr;
        }
    }
    return seq_buf[n - 1];
}
