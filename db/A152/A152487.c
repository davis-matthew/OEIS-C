/*
 * A152487  Triangle read by rows, 0<=k<=n: T(n,k) = Levenshtein distance of n
 *          and k in binary representation.
 * https://oeis.org/A152487
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, tabl
 * Formula:   a(n) = e(n) where e(n) is the Levenshtein distance between the
 *            binary representations of r and c, with r and c obtained from
 *            the flattened triangle index n: find r such that r*(r+1)/2 <= n
 *            < (r+1)*(r+2)/2, then c = n - r*(r+1)/2. e(n) = Levenshtein(r,
 *            c) on their binary strings
 * Range:     u8 n <= 9861, u16 n <= 9861, u32 n <= 9861, u64 n <= 9861,
 *            u128 n <= 9861, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Includes */
#include <stdint.h>
#include <stddef.h>

/* Helper: bit length of a 64‑bit unsigned integer (at least 1). */
static inline int bitlen_uint64(uint64_t x)
{
    int len = 0;
    do { len++; x >>= 1; } while (x);
    return len;
}

/* Compute Levenshtein distance between two unsigned integers
   interpreted as binary strings (most significant bit first). */
static inline seq_t lev_bin(uint64_t a, uint64_t b)
{
    int lenA = bitlen_uint64(a);
    int lenB = bitlen_uint64(b);

    /* Extract bits into arrays. */
    uint8_t bitsA[64], bitsB[64];
    for (int i = 0; i < lenA; ++i) {
        bitsA[i] = (a >> (lenA - 1 - i)) & 1;
    }
    for (int i = 0; i < lenB; ++i) {
        bitsB[i] = (b >> (lenB - 1 - i)) & 1;
    }

    /* DP arrays. */
    int prev[65] = {0};
    int cur[65]  = {0};

    /* Initialize first row (distance from empty A to prefixes of B). */
    for (int j = 0; j <= lenB; ++j) prev[j] = j;

    /* Main DP loops. */
    for (int i = 1; i <= lenA; ++i) {
        cur[0] = i;                     /* insertion cost on first row */
        for (int j = 1; j <= lenB; ++j) {
            int cost = (bitsA[i-1] == bitsB[j-1]) ? 0 : 1;
            int del = prev[j] + 1;       /* delete */
            int ins = cur[j-1] + 1;      /* insert */
            int sub = prev[j-1] + cost;  /* substitute */
            int best = del;
            if (ins < best) best = ins;
            if (sub < best) best = sub;
            cur[j] = best;
        }
        /* Copy current row to previous for next iteration. */
        for (int j = 0; j <= lenB; ++j)
            prev[j] = cur[j];
    }

    return (seq_t)prev[lenB];
}

/* Main function: a(n) for OEIS A152487. */
seq_t a(idx_t N)
{
    /* Find row r such that r*(r+1)/2 <= N < (r+1)*(r+2)/2. */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= N)
        ++r;

    /* Column index within the row. */
    idx_t c = N - r * (r + 1) / 2;

    /* Compute Levenshtein distance between binary representations of r and c. */
    return lev_bin((uint64_t)r, (uint64_t)c);
}
