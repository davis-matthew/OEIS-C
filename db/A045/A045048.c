/*
 * A045048  Numbers whose base-4 representation contains exactly two 0's and
 *          one 2.
 * https://oeis.org/A045048
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A045048: numbers whose base‑4 representation contains
   exactly two 0's and one 2.  For a given index n the function
   enumerates all admissible base‑4 numbers of the appropriate length,
   sorts them, and returns the nth element (1‑based). */

#include <stdlib.h>
#include <stdint.h>

/* Comparator for qsort on 64‑bit unsigned integers. */
static int cmp_uint64(const void *a, const void *b)
{
    uint64_t va = *(const uint64_t *)a;
    uint64_t vb = *(const uint64_t *)b;
    return (va < vb) ? -1 : (va > vb) ? 1 : 0;
}

/* External linkage as required. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    /* Find the base‑4 length L that contains the nth term. */
    idx_t L = 3;
    idx_t cumulative = 0;
    idx_t prevCum = 0;
    for (;; ++L) {
        /* C(L‑1,2) * (L‑2) * 2^(L‑3) */
        idx_t zerosChoices = (idx_t)(L - 1) * (L - 2) / 2;
        idx_t twoChoices   = L - 2;
        idx_t powHalf      = (idx_t)1 << (L - 3);   /* 2^(L‑3) */
        idx_t countL = zerosChoices * twoChoices * powHalf;
        if (cumulative + countL >= n) {
            prevCum = cumulative;
            break;
        }
        cumulative += countL;
    }

    /* Zero‑based offset within numbers of length L. */
    idx_t offset = n - prevCum - 1;

    /* Total number of admissible numbers of this length. */
    idx_t zerosChoices = (idx_t)(L - 1) * (L - 2) / 2;
    idx_t twoChoices   = L - 2;
    idx_t powHalf      = (idx_t)1 << (L - 3);
    idx_t cnt = zerosChoices * twoChoices * powHalf;

    /* Allocate array of 64‑bit values (all numbers fit comfortably). */
    uint64_t *arr = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    uint64_t *p = arr;

    /* Enumerate all digit patterns that satisfy the constraints. */
    for (idx_t z1 = 1; z1 <= L - 1; ++z1) {
        for (idx_t z2 = z1 + 1; z2 <= L - 1; ++z2) {
            for (idx_t t = 0; t < L; ++t) {
                if (t == z1 || t == z2) continue;          /* 2 cannot be at a zero position */

                /* Positions that are neither zeros nor the chosen 2. */
                idx_t remPos[64];
                idx_t remCnt = 0;
                for (idx_t i = 0; i < L; ++i) {
                    if (i == z1 || i == z2 || i == t) continue;
                    remPos[remCnt++] = i;
                }

                idx_t maskmax = (idx_t)1 << remCnt;        /* 2^(remaining positions) */
                for (idx_t mask = 0; mask < maskmax; ++mask) {
                    uint64_t val = 0;
                    for (idx_t i = 0; i < L; ++i) {
                        uint64_t digit;
                        if (i == z1 || i == z2)                 digit = 0;
                        else if (i == t)                       digit = 2;
                        else {                                 /* 1 or 3 */
                            idx_t k;
                            for (k = 0; k < remCnt; ++k)
                                if (remPos[k] == i) break;
                            digit = ((mask >> k) & 1) ? 3 : 1;
                        }
                        val = (val << 2) | digit;               /* val*4 + digit */
                    }
                    *p++ = val;
                }
            }
        }
    }

    /* Sort the generated numbers and select the required one. */
    qsort(arr, cnt, sizeof(uint64_t), cmp_uint64);
    uint64_t res64 = arr[offset];
    free(arr);

    return (seq_t)res64;   /* cast works for uint8_t..__int128 and arbitrary‐precision types */
}
