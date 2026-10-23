/*
 * A141299  a(n) = number (with repetition) of (not necessarily distinct)
 *          substrings in the binary representation of n that each occur
 *          multiple times.
 * https://oeis.org/A141299
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9941, u16 n <= 9941, u32 n <= 9941, u64 n <= 9941,
 *            u128 n <= 9941, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: For a given integer n, count all contiguous binary
   substrings that occur at least twice in the binary representation
   of n.  Each occurrence of such a substring contributes one to the
   count, so the result is the sum of the frequencies of all
   repeated substrings. */

#include <stdint.h>
#include <stddef.h>

/* Helper structure to keep track of substring occurrences */
struct entry {
    uint32_t val;   /* numeric value of the substring */
    unsigned len;   /* length of the substring in bits */
    unsigned freq;  /* how many times this substring has been seen */
};

seq_t a(idx_t n)
{
    /* For n = 0 we return 0; the sequence is defined for n >= 1. */
    if (n == 0) {
        return 0;
    }

    /* Compute the length L of the binary representation of n */
    size_t L = 0;
    idx_t tmp = n;
    while (tmp) {
        ++L;
        tmp >>= 1;
    }

    /* Allocate enough space for all possible substrings.
       For an L-bit number the maximum number of substrings is L*(L+1)/2. */
    size_t maxSubs = L * (L + 1) / 2;
    /* Limit to a reasonable constant for the small values we expect (L <= 64). */
    if (maxSubs <= 3000) {
        struct entry pool[3000];
        size_t count = 0;
        seq_t result = 0;

        for (size_t i = 0; i < L; ++i) {
            uint32_t val = 0;
            for (size_t j = i; j < L; ++j) {
                unsigned bit = (n >> (L - 1 - j)) & 1;
                val = (val << 1) | bit;
                unsigned len = (unsigned)(j - i + 1);

                /* Search for this substring in the pool */
                size_t k;
                for (k = 0; k < count; ++k) {
                    if (pool[k].len == len && pool[k].val == val) break;
                }
                if (k == count) {          /* new substring */
                    pool[count].len = len;
                    pool[count].val = val;
                    pool[count].freq = 1;
                    ++count;
                } else {                   /* seen before */
                    ++pool[k].freq;
                }
            }
        }

        /* Sum frequencies of substrings occurring multiple times */
        for (size_t i = 0; i < count; ++i) {
            if (pool[i].freq > 1) {
                result += (seq_t)pool[i].freq;
            }
        }
        return result;
    }

    /* Fallback: dynamic allocation for very large L (theoretically > 64) */
    struct entry *pool = (struct entry*)malloc(maxSubs * sizeof(struct entry));
    if (!pool) return 0; /* allocation failure */
    size_t count = 0;
    seq_t result = 0;

    for (size_t i = 0; i < L; ++i) {
        uint32_t val = 0;
        for (size_t j = i; j < L; ++j) {
            unsigned bit = (n >> (L - 1 - j)) & 1;
            val = (val << 1) | bit;
            unsigned len = (unsigned)(j - i + 1);

            size_t k;
            for (k = 0; k < count; ++k) {
                if (pool[k].len == len && pool[k].val == val) break;
            }
            if (k == count) {
                pool[count].len = len;
                pool[count].val = val;
                pool[count].freq = 1;
                ++count;
            } else {
                ++pool[k].freq;
            }
        }
    }

    for (size_t i = 0; i < count; ++i) {
        if (pool[i].freq > 1) result += (seq_t)pool[i].freq;
    }
    free(pool);
    return result;
}
