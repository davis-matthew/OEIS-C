/*
 * A050289  Zeroless pandigital numbers: numbers containing the digits 1-9
 *          (each appearing at least once) and no 0's.
 * https://oeis.org/A050289
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth zeroless pandigital number (digits 1–9, each at least
 *            once)
 * Range:     u8 none, u16 none, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow_uint.h"   /* seq_t pow_uint(uint64_t base, idx_t exp); */

/* Pre‑computed binomial coefficients C(n,k) for 0 ≤ n,k ≤ 9.  All fit in 64‑bit. */
static const seq_t binom[10][10] = {
    {1,0,0,0,0,0,0,0,0,0},
    {1,1,0,0,0,0,0,0,0,0},
    {1,2,1,0,0,0,0,0,0,0},
    {1,3,3,1,0,0,0,0,0,0},
    {1,4,6,4,1,0,0,0,0,0},
    {1,5,10,10,5,1,0,0,0,0},
    {1,6,15,20,15,6,1,0,0,0},
    {1,7,21,35,35,21,7,1,0,0},
    {1,8,28,56,70,56,28,8,1,0},
    {1,9,36,84,126,126,84,36,9,1}
};

/* Returns the number of ways to complete a length‑len suffix, given a
   bitmask of digits already used (bit d−1 set iff digit d has appeared). */
static seq_t count_suffix(unsigned len, uint16_t mask)
{
    unsigned missing = 9 - __builtin_popcount((unsigned)mask);

    if (len == 0)                 /* no positions left */
        return (missing == 0) ? 1 : 0;

    if (missing == 0)             /* all digits already present */
        return pow_uint(9, (idx_t)len);

    /* Inclusion–exclusion over the missing digits. */
    seq_t total = 0;
    for (unsigned i = 0; i <= missing; ++i) {
        seq_t term = binom[missing][i] * pow_uint(9 - i, (idx_t)len);
        if (i & 1)
            total -= term;
        else
            total += term;
    }
    return total;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* ---- 1. Determine the length L of the desired number. ---- */
    seq_t rem = n;          /* remaining index (1‑based) within current block */
    unsigned L   = 9;       /* minimal length */

    while (1) {
        seq_t cntL = count_suffix(L, 0);  /* all digits missing initially */
        if (rem <= cntL) break;
        rem -= cntL;
        ++L;
    }

    /* ---- 2. Unrank the rem‑th string of length L. ---- */
    unsigned mask = 0;
    int digits[200];            /* L will never exceed 200 for all practical n */
    seq_t rank = rem;           /* 1‑based rank inside current block */

    for (unsigned pos = 0; pos < L; ++pos) {
        unsigned len_rem = L - pos - 1;   /* positions remaining after this one */

        for (unsigned d = 1; d <= 9; ++d) {
            uint16_t new_mask = mask | (1u << (d - 1));
            seq_t    cnt      = count_suffix(len_rem, new_mask);

            if (rank > cnt) {
                rank -= cnt;
                continue;   /* try next higher digit */
            }

            /* Digit d is the correct one for this position. */
            digits[pos] = (int)d;
            mask = new_mask;
            break;
        }
    }

    /* ---- 3. Convert the digit array to the numeric value. ---- */
    seq_t value = 0;
    for (unsigned i = 0; i < L; ++i) {
        value = value * 10 + digits[i];
    }
    return value;
}
