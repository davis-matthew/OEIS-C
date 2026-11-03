/*
 * A059941  Take the n-th number which is just a sequence of 1's and 2's
 *          (A007931): if the first k digits in order are the same as the last
 *          k digits in order then put 1 in the k-th from right digit of a(n),
 *          otherwise put a zero.
 * https://oeis.org/A059941
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 62, u32 n <= 2046, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 58 terms through n = 8732 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A059941
 *  The n-th term is the decimal number whose k-th digit from the right
 *  is 1 iff the first k digits of the n‑th 1/2‑word (A007931) equal
 *  the last k digits of that word.
 *
 *  Implementation: compute the n‑th word, test each k, and build a
 *  decimal number by adding 10^(k-1) for every match.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/*  Return the index (0‑based) of the n‑th word and its length.
 *  The bits of the returned index represent the word: 0 → digit 1,
 *  1 → digit 2.  Positions are counted from most significant bit
 *  (leftmost digit).  The length of the word is stored in *len_out. */
static inline uint64_t word_bits(idx_t n, uint64_t *len_out)
{
    unsigned __int128 cum = 0;   /* cumulative number of words up to current length */
    unsigned __int128 add;       /* words of current length */
    uint64_t   L = 0;            /* current length */

    while (true) {
        ++L;
        add = (unsigned __int128)1 << L;   /* 2^L words of length L */
        cum += add;
        if ((unsigned __int128)n <= cum)
            break;
    }

    *len_out = L;
    unsigned __int128 prev = cum - add;            /* cumulative up to length L-1 */
    return (uint64_t)((unsigned __int128)n - prev - 1); /* 0‑based inner index */
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* seq_t is unsigned */

    uint64_t L;
    uint64_t pos = word_bits(n, &L);  /* pos contains the word bits */

    seq_t result = 0;
    seq_t multiplier = 1;             /* 10^(k-1) in decimal */

    for (uint64_t k = 1; k <= L; ++k) {
        bool equal = true;
        for (uint64_t j = 0; j < k && equal; ++j) {
            uint64_t b1 = (pos >> (L - 1 - j)) & 1;
            uint64_t b2 = (pos >> (k - 1 - j)) & 1;
            if (b1 != b2)
                equal = false;
        }
        if (equal)
            result += multiplier;
        multiplier *= 10;
    }
    return result;
}
