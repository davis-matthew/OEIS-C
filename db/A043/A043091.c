/*
 * A043091  Every string of 2 consecutive base 5 digits contains exactly 2
 *          distinct numbers.
 * https://oeis.org/A043091
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A043091: a(n) is the n‑th non‑negative integer whose base‑5
   expansion contains no pair of equal consecutive digits.
   a(0)=0, a(1)=1, a(2)=2, a(3)=3, a(4)=4, a(5)=5, … */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the special offset 0 explicitly. */
    if (n == 0)
        return (seq_t)0;

    /* Reduce the problem to positive integers by ignoring the first term. */
    idx_t idx = n - 1;          /* index among positive numbers 1,2,… */

    /* Determine the length L of the base‑5 representation of a(n). */
    idx_t L = 1;
    idx_t pow4 = 4;             /* 4^L  */
    while (idx >= pow4) {       /* idx ≥ number of L‑digit strings */
        idx -= pow4;
        ++L;
        pow4 *= 4ULL;           /* next power 4^(L+1) */
    }

    /* For length 1 we have the digits 1,2,3,4. */
    if (L == 1) {
        /* idx ∈ {0,1,2,3} */
        return (seq_t)(1 + idx);
    }

    /* For L ≥ 2 construct the base‑5 digits. */
    uint8_t digits[64];         /* enough for all practical L (≤ 10 for n≤10⁴) */
    idx_t rem = idx;
    idx_t powRem = pow4 / 4;    /* 4^(L‑1) , number of strings for each first digit */

    /* First digit: choose one of 1,2,3,4 */
    idx_t dindex = rem / powRem;     /* 0 … 3 */
    rem %= powRem;
    uint8_t first = (uint8_t)(1 + dindex);
    digits[0] = first;
    uint8_t prev = first;

    /* Remaining digits: cannot equal previous. */
    for (idx_t pos = 1; pos < L; ++pos) {
        powRem /= 4;                 /* 4^(remaining positions) */
        idx_t digit_index = rem / powRem; /* 0 … 3 */
        rem %= powRem;

        uint8_t cand = (uint8_t)digit_index;
        if (cand >= prev)            /* skip the forbidden previous digit */
            ++cand;
        digits[pos] = cand;
        prev = cand;
    }

    /* Convert the base‑5 representation to a decimal value. */
    seq_t value = 0;
    for (idx_t pos = 0; pos < L; ++pos) {
        value = value * (seq_t)5 + (seq_t)digits[pos];
    }

    return value;
}
