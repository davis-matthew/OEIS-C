/*
 * A044986  Numbers whose base-3 representation contains exactly four 0's and
 *          one 1.
 * https://oeis.org/A044986
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   allocate_seq_array, cnt_len
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A044986:
   numbers whose base‑3 representation contains exactly four 0's and one 1. */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/allocate_seq_array.h"
#include "../utils/cnt_len.h"

/* Allocate an array of seq_t that is safe in C and C++. */

/* Free an array allocated by allocate_seq_array. */
static void free_seq_array(seq_t *p)
{
#ifdef __cplusplus
    delete[] p;
#else
    free(p);
#endif
}

static int seq_compare(const void *a, const void *b)
{
    const seq_t *x = (const seq_t *)a;
    const seq_t *y = (const seq_t *)b;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* index 0 is outside the offset */

    /* Find the minimum length needed to contain at least n terms. */
    size_t total = 0U;
    int L = 4;
    while (total < n) {
        ++L;
        if (L < 5) continue;
        total += cnt_len(L);
    }

    seq_t *arr = allocate_seq_array(total);
    if (!arr) return 0;

    seq_t *pow3 = allocate_seq_array(L + 1);
    if (!pow3) {
        free_seq_array(arr);
        return 0;
    }

    pow3[0] = (seq_t)1;
    for (int i = 1; i <= L; ++i) {
        pow3[i] = pow3[i - 1] * (seq_t)3;
    }

    size_t pos = 0U;

    for (int len = 5; len <= L; ++len) {
        seq_t base = pow3[len] - (seq_t)1;

        for (int z1 = 0; z1 <= len - 5; ++z1)
        for (int z2 = z1 + 1; z2 <= len - 4; ++z2)
        for (int z3 = z2 + 1; z3 <= len - 3; ++z3)
        for (int z4 = z3 + 1; z4 <= len - 2; ++z4) {

            seq_t sumZero = pow3[z1] + pow3[z2] + pow3[z3] + pow3[z4];

            for (int p = 0; p < len; ++p) {
                if (p == z1 || p == z2 || p == z3 || p == z4) continue;

                seq_t val = base - ((seq_t)2 * sumZero + pow3[p]);
                arr[pos++] = val;
            }
        }
    }

    /* The number of generated terms should match the expected total. */
    if (pos != total) {
        free_seq_array(pow3);
        free_seq_array(arr);
        return 0;
    }

    qsort(arr, total, sizeof(seq_t), seq_compare);

    seq_t result = arr[n - 1];

    free_seq_array(pow3);
    free_seq_array(arr);
    return result;
}
