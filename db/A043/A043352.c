/*
 * A043352  Numbers having four 0's in base 5.
 * https://oeis.org/A043352
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 37, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: generate the nth integer whose base‑5 representation
 * contains exactly four zeros.  The sequence is OEIS A043352. */

#include <stdint.h>

/* compute n choose k for small k (k <= 4) */
static idx_t comb(idx_t n, idx_t k)
{
    idx_t r = 1;
    for (idx_t i = 1; i <= k; ++i) {
        r = r * (n - k + i) / i;
    }
    return r;
}

/* 4^exp (exp <= 20 for the sizes we need) */
static idx_t pow4(idx_t exp)
{
    idx_t p = 1;
    while (exp--) p *= 4;
    return p;
}

/* number of ways to complete a number when we have rempos positions
 * left to fill and remZeros zeros still to place */
static idx_t count_completions(idx_t rempos, idx_t remZeros)
{
    if (remZeros > rempos) return 0;
    return comb(rempos, remZeros) * pow4(rempos - remZeros);
}

seq_t a(idx_t n)
{
    /* Find the number of digits d for the nth term */
    idx_t cumulative = 0;          /* total terms counted so far   */
    idx_t d = 5;                   /* minimal number of digits    */

    while (1) {
        idx_t cnt = comb(d - 1, 4) * pow4(d - 4); /* terms with d digits */
        if (cumulative + cnt >= n) break;
        cumulative += cnt;
        ++d;
    }

    idx_t offset = n - cumulative - 1;        /* zero‑based rank within length d */

    /* Construct the base‑5 digits from most significant to least */
    seq_t result = 0;
    idx_t remZeros = 4;                      /* zeros still to place   */

    for (idx_t pos = d; pos-- > 0; ) {
        /* Determine the allowed digit range for this position */
        int start = 0, end = 4;
        if (pos == d - 1) start = 1;          /* most significant cannot be 0 */

        for (int dig = start; dig <= end; ++dig) {
            if (dig == 0 && remZeros == 0) continue;  /* cannot place another 0 */

            idx_t newRemZeros = remZeros - (dig == 0 ? 1 : 0);
            idx_t remainingPositions = pos;           /* after this digit */
            idx_t block = count_completions(remainingPositions, newRemZeros);

            if (offset < block) {
                /* this digit is chosen */
                result = result * 5 + dig;
                remZeros = newRemZeros;
                break;
            } else {
                offset -= block;
            }
        }
    }
    return result;
}
