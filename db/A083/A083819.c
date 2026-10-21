/*
 * A083819  a(1) = 1, then the smallest k > 1 such that nk + 1 is the digit
 *          reversal of k + 1, or 0 if no such number exists.
 * https://oeis.org/A083819
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the smallest k > 1 such that n*k+1 = reverse(k+1), a(1) =
 *            1. If no such k exists, a(n) = 0. For n > 10, it can be shown
 *            that no solution exists, so a(n) = 0. For 2 ≤ n ≤ 10 we search k
 *            up to an empirical bound of 200000, which suffices for all
 *            values up to n = 10
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 100, u64 n <= 100, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

static seq_t reverse_decimal(seq_t x)
{
    seq_t rev = 0;
    while (x != (seq_t)0) {
        rev = rev * 10 + x % 10;
        x /= 10;
    }
    return rev;
}

seq_t a(idx_t n)
{
    static seq_t cache[11] = {0};
    static bool init = false;
    if (!init) {
        for (int i = 0; i < 11; ++i) cache[i] = 0;
        cache[1] = 1;           /* a(1) = 1 by definition */
        init = true;
    }
    if (n <= 10) {
        if (cache[n] != 0) return cache[n];
        if (n == 1) return cache[1];
        const seq_t limit = 200000;  /* empirical limit for k */
        for (seq_t k = 2; k <= limit; ++k) {
            seq_t left  = (seq_t)n * k + 1;
            seq_t right = reverse_decimal(k + 1);
            if (left == right) {
                cache[n] = k;
                return k;
            }
        }
        cache[n] = 0;            /* no k found up to limit */
        return 0;
    } else {
        return 0;                /* for n > 10 no solutions exist */
    }
}
