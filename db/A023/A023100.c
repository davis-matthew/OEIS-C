/*
 * A023100  a(n) is least k such that k and 8k are anagrams in base n (written
 *          in base 10).
 * https://oeis.org/A023100
 *
 * Offset:    9    (a(9) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = least k such that k and 8k are anagrams in base n. *
 *            Computation: brute force search over multiples of
 *            (n-1)/gcd(n-1,7)
 * Range:     u8 none, u16 n <= 9, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Validated: partial, pass, 37 terms through n = 45 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Check whether the digits of k and 8*k (in base 'base') form the same multiset. */
static bool is_anagram(seq_t k, unsigned int base)
{
    static unsigned int counter[10001];
    static unsigned short usedIdx[10001];
    unsigned usedCount = 0;

    seq_t tmp = k;
    seq_t baseSeq = (seq_t)base;

    /* Count digits of k */
    while (tmp > 0) {
        seq_t rem = tmp % baseSeq;
        unsigned digit = (unsigned)rem;    /* base ≤ 10000 */
        if (counter[digit] == 0) {
            usedIdx[usedCount++] = digit;
        }
        counter[digit]++;
        tmp /= baseSeq;
    }

    /* Subtract digits of 8*k */
    seq_t tmp2 = k * (seq_t)8;
    while (tmp2 > 0) {
        seq_t rem = tmp2 % baseSeq;
        unsigned digit = (unsigned)rem;
        if (counter[digit] == 0) {
            /* restore counters before returning */
            for (unsigned i = 0; i < usedCount; ++i)
                counter[usedIdx[i]] = 0;
            return false;
        }
        counter[digit]--;
        tmp2 /= baseSeq;
    }

    /* Ensure all counts are zero */
    for (unsigned i = 0; i < usedCount; ++i) {
        if (counter[usedIdx[i]] != 0) {
            for (unsigned j = 0; j < usedCount; ++j)
                counter[usedIdx[j]] = 0;
            return false;
        }
    }

    /* Reset counters for next call */
    for (unsigned i = 0; i < usedCount; ++i)
        counter[usedIdx[i]] = 0;

    return true;
}

/* Euclidean gcd for 64‑bit unsigned integers */
static uint64_t gcd_uint64(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n < 9)
        return 0; /* undefined for n < 9, arbitrary return */

    uint64_t base = (uint64_t)n;
    uint64_t baseMinus1 = base - 1;
    uint64_t g = gcd_uint64(baseMinus1, 7ULL);
    uint64_t step_val = baseMinus1 / g;

    seq_t step = (seq_t)step_val;
    seq_t k = step; /* first candidate */

    while (true) {
        if (is_anagram(k, (unsigned)base))
            return k;
        k += step;
    }
}
