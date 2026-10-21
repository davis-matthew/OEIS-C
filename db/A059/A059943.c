/*
 * A059943  Toss a fair coin and calculate the expected time until the n-th
 *          possible finite sequence of Heads and Tails first appears (ordered
 *          by length of sequence and alphabetical order so H, T, HH, HT, TH,
 *          TT, HHH, etc.).
 * https://oeis.org/A059943
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nice, nonn
 * Formula:   not stated
 * Range:     u8 n <= 254, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A059943: Expected time for the n‑th binary string to first appear
   in a fair coin toss sequence.
   
   Formula: a(n) = Σ_{k∈B(n)} 2^k ,
   where B(n) is the set of lengths of all non‑empty prefixes that are
   also suffixes of the n‑th binary string (including the full length).
   The n‑th string is the n‑th element of the ordering by increasing
   length and lexicographic order with H<'T'. */

/* The types seq_t and idx_t are supplied by the build; do not define them. */
#include <stdint.h>
#include <stdbool.h>

/* Return the expected waiting time for the n‑th binary string. */
seq_t a(idx_t n)
{
    /* 1. Find the length L of the n‑th string. */
    idx_t L = 1;
    while (true) {
        /* cumulative count of strings up to length L  : 2^{L+1} - 2 */
        __uint128_t cum = ((__uint128_t)1 << (L + 1)) - 2;
        if ((__uint128_t)n <= cum)
            break;
        ++L;
    }

    /* 2. Compute offset of n within the block of length L. */
    __uint128_t prev_cum = ((__uint128_t)1 << L) - 2;   /* count up to length L-1 */
    idx_t offset = (idx_t)((__uint128_t)n - prev_cum - 1);   /* 0‑based */

    /* 3. Build the string bits: 0 → H, 1 → T. */
    /* We allocate a small static array; for the limits required by the
       typical use (< 2^64) L never exceeds 64, but we allow up to 128 for safety. */
    uint8_t s[128];
    for (idx_t i = 0; i < L; ++i) {
        s[i] = (uint8_t)((offset >> (L - 1 - i)) & 1U);
    }

    /* 4. Compute the prefix function (failure function) for KMP. */
    uint8_t pi[128];
    pi[0] = 0;
    for (idx_t i = 1; i < L; ++i) {
        idx_t j = pi[i - 1];
        while (j > 0 && s[i] != s[j])
            j = pi[j - 1];
        if (s[i] == s[j])
            ++j;
        pi[i] = (uint8_t)j;
    }

    /* 5. Sum 2^k over all border lengths. */
    __uint128_t sum = 0;
    idx_t k = L;
    while (k > 0) {
        sum += ((__uint128_t)1 << k);
        k = pi[k - 1];   /* next shorter border */
    }

    /* 6. Return the result; cast truncates if it doesn't fit in seq_t. */
    return (seq_t)sum;
}
