/*
 * A038452  Sums of 10 distinct powers of 10.
 * https://oeis.org/A038452
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth smallest number consisting of exactly ten decimal
 *            digits equal to 1 (and zeros elsewhere)
 * Range:     u8 none, u16 none, u32 n <= 1, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

/* Computes binomial coefficient C(n,k) for 0 <= k <= n using seq_t.
   The function is safe for n up to the size needed by this problem
   (≈20). */
static inline seq_t binom(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t result = 1;
    for (idx_t i = 1; i <= k; ++i) {
        result = result * (seq_t)(n - k + i);
        result = result / (seq_t)i;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for the OEIS offset, */
                                   /* return 0 for safety */
    /* Find minimal length L such that C(L,10) >= n. */
    idx_t L = 10;
    while (true) {
        seq_t cnt = binom(L, 10);
        if (cnt >= n) break;
        ++L;
    }

    /* Index within numbers of length L (1‑based). */
    seq_t previous = binom(L - 1, 10);
    seq_t k = (seq_t)n - previous;   /* 1‑based rank inside length L */

    /* Build the digit pattern.  digits[i] = 1 means   10^i is present. */
    bool digits[128] = {0};
    digits[L - 1] = true;            /* leading digit is always 1 */

    idx_t needed = 9;                /* remaining ones to place */
    /* Process positions from most significant trailing digit downwards. */
    for (idx_t pos = 0; pos < L - 1; ++pos) {
        /* The actual index of this digit in the array is L-2 - pos. */
        idx_t idx = L - 2 - pos;
        idx_t remaining = idx + 1;   /* positions left including this one */

        seq_t waysZero = 0;
        if (remaining - 1 >= needed) {
            waysZero = binom(remaining - 1, needed);
        }

        if (waysZero >= k) {
            /* This digit is 0. */
            digits[idx] = false;
        } else {
            /* This digit is 1. */
            digits[idx] = true;
            k -= waysZero;
            if (needed > 0) --needed;
        }
    }

    /* Convert digit pattern to numeric value. */
    seq_t value = 0;
    seq_t pow10 = 1;
    for (idx_t i = 0; i < L; ++i) {
        if (digits[i]) value += pow10;
        pow10 *= (seq_t)10;
    }
    return value;
}
