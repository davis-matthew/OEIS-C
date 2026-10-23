/*
 * A180039  Combinatorial tetrahedral numbers.
 * https://oeis.org/A180039
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = (∑_{i=1}^n 10^{T_i} – n) / 9, where T_i = i*(i+1)/2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation builds the integer Σ10^{T_i} by setting a bit in each
   decimal position T_i (which is 1 at those positions and 0 elsewhere),
   converts it to a seq_t value, then performs the final arithmetic. */

#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;
    }

    /* Compute the largest triangular number T_n = n*(n+1)/2. */
    idx_t T = n * (n + 1) / 2;
    size_t len = (size_t)T + 1;          /* number of decimal positions needed */

    /* Allocate a byte array where each entry is 0 or 1 (bit at that position). */
    uint8_t *bits = (uint8_t *)calloc(len, sizeof(uint8_t));
    if (!bits) return (seq_t)0;          /* out‑of‑memory guard */

    /* Mark the positions T_i.  Use a running triangular sum to avoid repeated
       multiplications. */
    idx_t t = 0;
    for (idx_t i = 1; i <= n; ++i) {
        t += i;            /* t == i*(i+1)/2  */
        bits[t] = 1;
    }

    /* Convert the bit array into a seq_t value. We process from the most
       significant position to the least to arise at the correct numeric
       value in base 10. */
    size_t msb = (size_t)t;   /* highest non‑zero bit index */
    seq_t sum = 0;
    for (size_t pos = msb; ; --pos) {
        sum = sum * (seq_t)10 + (seq_t)bits[pos];
        if (pos == 0) break;
    }

    free(bits);

    /* a(n) = (sum – n) / 9 */
    sum -= (seq_t)n;
    sum /= (seq_t)9;

    return sum;
}
