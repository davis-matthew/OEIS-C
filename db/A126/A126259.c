/*
 * A126259  Numbers that are divisible by the number of letters in their
 *          English name, excluding spaces and hyphens;.
 * https://oeis.org/A126259
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 1811, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS formula: a(n) = the n‑th positive integer divisible by the
 *  number of letters in its British English name (including “and”).
 *
 *  The function scans integers sequentially until the n‑th qualifying
 *  number is found.  It uses no external libraries apart from the
 *  standard headers and the seq_t / idx_t types supplied by the build.
 *
 *  Letter‑count tables are derived from the mapping of English words
 *  to letter lengths, respecting the British convention that “and”
 *  is inserted between the hundreds and the lower part of a number.
 */

#include <stdint.h>
#include <stddef.h>

/* ---------- tables for single‑digit, two‑digit, tens and magnitudes ---------- */
static const int len_1to9[9]  = {3,3,5,4,4,3,5,5,4};          /* one … nine */
static const int len_1to19[19] = {3,3,5,4,4,3,5,5,4,3,6,6,8,8,7,7,9,8,8};
/* indices 0..7 correspond to twenty … ninety */
static const int len_tens[8] = {6,6,5,5,5,7,6,6};
static const int magnitude_len[10] = {0,8,7,7,8,12,12,10,10,9}; /* "", thousand … octillion */

/* ---------- helper: letters in a 0…999 group -------------------------------- */
static inline int letters3(uint32_t g)
{
    if (g == 0) return 0;
    int len = 0;
    uint32_t h = g / 100;
    uint32_t rem = g % 100;

    if (h) {
        len += len_1to9[h-1];
        len += 7;                 /* "hundred" */
        if (rem) len += 3;        /* "and" */
    }
    if (rem) {
        if (rem < 20) {
            len += len_1to19[rem-1];
        } else {
            uint32_t t = rem / 10;
            uint32_t o = rem % 10;
            len += len_tens[t-2];
            if (o) len += len_1to9[o-1];
        }
    }
    return len;
}

/* ---------- total number of letters for a non‑negative 64‑bit integer ------- */
static inline int letter_count_u64(uint64_t n)
{
    if (n == 0) return 0;
    uint64_t groups[10];
    int gcount = 0;
    uint64_t tmp = n;

    while (tmp) {
        groups[gcount++] = tmp % 1000ULL;
        tmp /= 1000ULL;
    }

    int len = 0;
    for (int i = gcount-1; i >= 0; --i) {
        int g = (int)groups[i];
        if (g == 0) continue;
        len += letters3(g);
        if (i > 0) len += magnitude_len[i];
    }

    /* British convention: insert “and” if the number is >999 and the
       lower‑3‑digit group is <100. */
    if (n >= 1000ULL && groups[0] != 0ULL && groups[0] < 100ULL) {
        len += 3;   /* "and" */
    }
    return len;
}

/* ---------- sequence implementation -------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for n==0, but harmless */
    idx_t  found = 0;
    seq_t  last  = 0;

    for (idx_t x = 1; found < n; ++x) {
        int lc = letter_count_u64((uint64_t)x);
        if (lc && (x % (idx_t)lc == 0)) {
            ++found;
            last = (seq_t)x;
        }
    }
    return last;
}
