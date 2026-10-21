/*
 * A080570  Number of 1's in partition of n-th Ulam number into sum of two
 *          distinct Ulam numbers.
 * https://oeis.org/A080570
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of 1's in the recursive binary decomposition of
 *            the nth Ulam number into sums of two distinct earlier Ulam
 *            numbers
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <string.h>

#define MAX_ULAM 40000          /* enough for n up to ~10000 */
#define MAX_SUM  1000000         /* enough for all sums of Ulam numbers up to above limit */

static uint64_t ulam[MAX_ULAM];
static unsigned short pu1[MAX_ULAM];
static unsigned short pu2[MAX_ULAM];
static seq_t ones[MAX_ULAM];

static int    cnt[MAX_SUM];
static unsigned short cnt_p1[MAX_SUM];
static unsigned short cnt_p2[MAX_SUM];

static unsigned int len = 0;     /* number of Ulam numbers generated */

/* initialize arrays once */
static void init(void) {
    if (len != 0) return;          /* already initialized */

    /* zero count arrays */
    memset(cnt,   0, sizeof(cnt));
    memset(cnt_p1,0, sizeof(cnt_p1));
    memset(cnt_p2,0, sizeof(cnt_p2));

    /* first two Ulam numbers */
    ulam[0] = 1;  pu1[0] = pu2[0] = 0;   ones[0] = (seq_t)1;
    ulam[1] = 2;  pu1[1] = pu2[1] = 0;   ones[1] = (seq_t)0;

    len = 2;

    /* initial sum 1+2 = 3 */
    cnt[3]      = 1;
    cnt_p1[3]   = 0;   /* index of 1 */
    cnt_p2[3]   = 1;   /* index of 2 */
}

/* generate Ulam numbers up to index n (1‑based) and compute ones[] */
static void generate(idx_t n) {
    init();
    while (len < (unsigned)n) {
        /* find next candidate: smallest integer > last with cnt==1 */
        uint64_t cand = ulam[len-1] + 1;
        while (cand >= MAX_SUM || cnt[(size_t)cand] != 1) cand++;

        /* indices of the unique pair that sums to cand */
        unsigned short i1 = cnt_p1[(size_t)cand];
        unsigned short i2 = cnt_p2[(size_t)cand];

        /* append new Ulam number and compute ones value */
        ulam[len] = cand;
        pu1[len]  = i1;
        pu2[len]  = i2;
        ones[len] = ones[i1] + ones[i2];
        len++;

        /* update counts with sums involving the new element */
        unsigned int newIdx = len - 1;
        for (unsigned int j = 0; j < newIdx; ++j) {
            uint64_t sum = ulam[newIdx] + ulam[j];
            if (sum >= MAX_SUM) continue;          /* should not happen for required range */
            if (cnt[(size_t)sum] == 0) {
                cnt_p1[(size_t)sum] = newIdx;
                cnt_p2[(size_t)sum] = j;
            }
            cnt[(size_t)sum]++;                    /* increment count */
        }
    }
}

/* external function required by the build */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* offset 1; behaviour below does not matter */
    generate(n);
    return ones[n-1];              /* 1‑based indexing */
}
