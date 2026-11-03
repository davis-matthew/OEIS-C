/*
 * A227130  Numbers k for which there is an even number of nonzero digits when
 *          k is written in the factorial base (A007623).
 * https://oeis.org/A227130
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th integer k whose factorial-base representation
 *            contains an even number of non‑zero digits
 * Range:     u8 none, u16 none, u32 n <= 2520, u64 n <= 2520, u128 n <= 2520,
 *            big any n
 * Validated: standard, pass, 79 terms through n = 2416 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* Helper: test whether k has an even number of non‑zero factorial‑base
   digits.  Returns true if the count is even. */
static bool even_nonzero_factorial_digits(seq_t k)
{
    /* Precompute factorials up to 20! (fits in 64‑bit).  We compute
       them once on the first call, storing in static array. */
    static seq_t facts[21];
    static bool init = false;
    if (!init) {
        facts[1] = (seq_t)1;          /* 1! */
        for (size_t i = 2; i <= 20; ++i)
            facts[i] = facts[i-1] * (seq_t)i;
        init = true;
    }

    /* Find the largest factorial <= k. */
    size_t i;
    for (i = 20; i > 0; --i)
        if (facts[i] <= k)
            break;

    /* Scan digits from highest to lowest, toggling parity when a
       non‑zero digit is found. */
    int parity = 0;                     /* 0 = even, 1 = odd */
    for (; i > 0; --i) {
        seq_t digit = k / facts[i];
        if (digit != 0)
            parity ^= 1;
        k %= facts[i];
    }
    return parity == 0;
}

/* Main function required by the problem. */
seq_t a(idx_t n)
{
    /* n is 1‑based.  Enumerate integers k starting from 0 until we
       find the n‑th one with an even number of non‑zero factorial‑base
       digits. */
    idx_t count = 0;
    seq_t k = (seq_t)0;
    while (1) {
        if (even_nonzero_factorial_digits(k)) {
            ++count;
            if (count == n)
                return k;
        }
        ++k;
    }
}
