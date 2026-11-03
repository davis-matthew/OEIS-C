/*
 * A043958  Numbers k such that 5 and 7 occur juxtaposed in the base-8
 *          representation of k but not of k+1.
 * https://oeis.org/A043958
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Validated: partial, pass, 43 terms through n = 43 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A043958
   Numbers k such that 5 and 7 occur juxtaposed in the base‑8 representation
   of k but not of k+1.
   The function returns the nth term (offset 1) by brute‑force scanning,
   which is adequate for n ≈ 10000 within the required time limit. */
#include <stdbool.h>

static inline bool has_adjacent_57(idx_t x)
{
    int prev = -1;
    while (x != 0) {
        int d = x % 8;
        if ((prev == 5 && d == 7) || (prev == 7 && d == 5))
            return true;
        prev = d;
        x /= 8;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                    /* behaviour below offset irrelevant */

    idx_t count = 0;
    idx_t k = 0;

    while (count < n) {
        ++k;
        if (has_adjacent_57(k) && !has_adjacent_57(k + 1))
            ++count;
    }

    return (seq_t)k;                  /* cast to seq_t (may be arbitrary‑precision) */
}
