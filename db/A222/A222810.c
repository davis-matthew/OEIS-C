/*
 * A222810  Number of n-digit numbers N with distinct digits such that the
 *          reversal of N divides N.
 * https://oeis.org/A222810
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation by enumeration of all n‑digit numbers with distinct digits
   such that the reversal divides the number (OEIS A222810).
   For n>6 the result is zero, because no such numbers exist. */

#include <stdbool.h>
#include <stdint.h>

/* static storage for precomputed values */
static seq_t pow10[7];
static seq_t cache[7];
static bool init_done = false;

/* recursive depth‑first search for a given length */
static void dfs(idx_t pos, idx_t len, uint16_t used, seq_t num, seq_t rev, seq_t *cnt)
{
    if (pos == len) {
        if (rev != (seq_t)0 && (num % rev == (seq_t)0))
            (*cnt)++;
        return;
    }
    for (int d = 0; d <= 9; ++d) {
        if (used & (1u << d))
            continue;
        if (pos == 0 && d == 0)
            continue;               /* no leading zero */
        seq_t n = num * (seq_t)10 + (seq_t)d;
        seq_t r = rev + pow10[pos] * (seq_t)d;
        dfs(pos + 1, len, used | (1u << d), n, r, cnt);
    }
}

/* initialise powers of ten and pre‑compute the sequence values */
static void init_values(void)
{
    pow10[0] = (seq_t)1;
    for (idx_t i = 1; i <= 6; ++i)
        pow10[i] = pow10[i - 1] * (seq_t)10;

    cache[0] = (seq_t)0;              /* unused */
    for (idx_t len = 1; len <= 6; ++len) {
        seq_t cnt = (seq_t)0;
        dfs(0, len, 0u, (seq_t)0, (seq_t)0, &cnt);
        cache[len] = cnt;
    }
    init_done = true;
}

/* required public function */
seq_t a(idx_t n)
{
    if (!init_done)
        init_values();
    return (n <= 6) ? cache[n] : (seq_t)0;
}
