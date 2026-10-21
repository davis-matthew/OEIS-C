/*
 * A225577  Least integer m>1 such that 1^2,2^2,...,n^2 are pairwise
 *          incongruent modulo 2^m-1.
 * https://oeis.org/A225577
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for minimal m>1 such that 1^2,2^2,…,n^2 are pairwise incongruent
   modulo 2^m-1.  The algorithm tries increasing m, checks distinctness
   of the squares by sorting the reduced residues.  It stops when distinct
   residues are found and returns that m as seq_t. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Compare two uint64_t values for qsort */
static int cmp_uint64(const void *a, const void *b)
{
    uint64_t va = *(const uint64_t *)a;
    uint64_t vb = *(const uint64_t *)b;
    if (va < vb) return -1;
    if (va > vb) return 1;
    return 0;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n<1 */

    /* Convert to 64‑bit for internal arithmetic */
    idx_t nidx = n;
    uint64_t nn = (uint64_t)nidx * (uint64_t)nidx;   /* n^2 */

    for (unsigned m = 2; ; m++) {
        /* M = 2^m - 1 ;  m <= 63 guaranteed by the input range */
        uint64_t M = ((uint64_t)1 << m) - 1ULL;

        /* If M > n^2, all squares are automatically distinct */
        if (M > nn) return (seq_t)m;

        /* Allocate array of reduced residues */
        uint64_t *arr = (uint64_t *)malloc((size_t)nidx * sizeof(uint64_t));
        if (!arr) return (seq_t)0;  /* allocation failure – unlikely */

        for (idx_t i = 1; i <= nidx; ++i) {
            uint64_t sq = (uint64_t)i * (uint64_t)i;
            arr[i - 1] = sq % M;
        }

        /* Sort and check for duplicates */
        qsort(arr, (size_t)nidx, sizeof(uint64_t), cmp_uint64);

        bool distinct = true;
        for (idx_t i = 1; i < nidx; ++i) {
            if (arr[i - 1] == arr[i]) {  /* duplicate found */
                distinct = false;
                break;
            }
        }

        free(arr);

        if (distinct) return (seq_t)m;   /* found minimal m */
    }
}
