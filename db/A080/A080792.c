/*
 * A080792  Numbers which when seen in a mirror are different numbers.
 * https://oeis.org/A080792
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 56, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A080792
   Formula: a(n) is the n‑th positive integer whose decimal digits are taken from
            {0,1,2,5,8} and which is *not* unchanged by the “mirror” operation:
            reverse the order of the digits and replace each digit according to
            0↔0, 1↔1, 2↔5, 5↔2, 8↔8.  Only numbers that change under this
            operation are listed. */

#include <stddef.h>
#include <stdlib.h>

/* Mapping of single decimal digits when mirrored.  Digits not appearing
   in the set {0,1,2,5,8} are never accessed, but are given
   placeholder values to keep the compiler happy. */
static const int mirrorMap[10] = {0,1,5,-1,-1,2,-1,-1,8,-1};

/* First digits allowed for a number (most significant digit cannot be 0). */
static const unsigned char firstDigits[4] = {1,2,5,8};
/* All remaining positions may also use 0. */
static const unsigned char allDigits[5]   = {0,1,2,5,8};

/* Dynamically‑grown array that will hold the sequence values in order. */
static seq_t   *seqVals   = NULL;
static size_t   seqSize   = 0;  /* number of values already stored   */
static size_t   seqCap    = 0;  /* allocated capacity of seqVals      */
static int      maxLenGen = 0;  /* maximum digit length already generated */

/* ----------------------------------------------------------------- */
/* Grow the dynamic array until it can contain at least 'need' elements. */
static void growSeq(size_t need)
{
    if (seqCap >= need) return;
    size_t newCap = seqCap ? seqCap * 2 : 4;
    while (newCap < need) newCap <<= 1;
    seqVals = (seq_t *)realloc(seqVals, newCap * sizeof(seq_t));
    seqCap  = newCap;
}

/* ----------------------------------------------------------------- */
/* Compute the mirror of a number that contains only the allowed digits. */
static seq_t mirrorOf(seq_t cur)
{
    seq_t mirror = 0;
    seq_t tmp    = cur;
    while (tmp > 0) {
        int d   = (int)(tmp % 10);
        int md  = mirrorMap[d];
        mirror  = mirror * 10 + md;
        tmp    /= 10;
    }
    /* Leading zeros produced by the mapping are naturally discarded when
       the result is stored as an integer. */
    return mirror;
}

/* ----------------------------------------------------------------- */
/* Recursively generate all numbers of a fixed digit length 'len'.
   Numbers are generated in increasing numeric order because the digit
   arrays are sorted ascending and recursion proceeds from most
   significant to least significant position.  When a complete number
   is formed, if it differs from its mirror, it is appended to seqVals. */
static void recDigits(int pos, int len, seq_t cur)
{
    if (pos == len) {
        if (cur != mirrorOf(cur)) {
            growSeq(seqSize + 1);
            seqVals[seqSize++] = cur;
        }
        return;
    }

    const unsigned char *digits = (pos == 0) ? firstDigits : allDigits;
    int count = (pos == 0) ? 4 : 5;

    for (int i = 0; i < count; ++i) {
        seq_t newVal = cur * 10 + digits[i];
        recDigits(pos + 1, len, newVal);
    }
}

/* ----------------------------------------------------------------- */
/* Generate all numbers of digit length 'len'. */
static void generateLen(int len)
{
    recDigits(0, len, 0);
}

/* ----------------------------------------------------------------- */
/* Ensure that seqVals contains at least 'n' terms.  This function will
   generate numbers of increasing length until enough terms have been
   collected. */
static void ensureSize(idx_t n)
{
    while (seqSize < (size_t)n) {
        ++maxLenGen;
        generateLen(maxLenGen);
    }
}

/* ----------------------------------------------------------------- */
/* The required external function.  It may be called in any order of
   indices; the sequence is stored internally so that repeated calls
   are fast. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* undefined for indices < 1 */
    ensureSize(n);
    return seqVals[n - 1];
}
