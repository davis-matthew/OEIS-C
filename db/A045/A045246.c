/*
 * A045246  Numbers whose base-5 representation contains exactly three 1's and
 *          two 3's.
 * https://oeis.org/A045246
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 37, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: numbers whose base‑5 representation contains exactly three 1's and two 3's.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Ensure capacity of the static array that caches the sequence terms. */
static void ensure_capacity(seq_t **array, size_t *cap, size_t needed)
{
    if (*cap >= needed)
        return;
    size_t new_cap = (*cap == 0) ? 16 : (*cap * 2);
    if (new_cap < needed)
        new_cap = needed;

#ifdef __cplusplus
    /* Allocate a new array, copy old values, and delete the old array. */
    seq_t *tmp = new seq_t[new_cap];
    for (size_t i = 0; i < *cap; ++i)
        tmp[i] = (*array)[i];
    delete [] *array;
#else
    seq_t *tmp = (seq_t *)realloc(*array, new_cap * sizeof(seq_t));
#endif
    *array = tmp;
    *cap   = new_cap;
}

/* Return the n-th term of the sequence (n ≥ 1). */
seq_t a(idx_t n)
{
    static seq_t *terms = NULL;        /* Cached sequence values                  */
    static size_t terms_len = 0;       /* Number of cached entries                */
    static size_t terms_cap = 0;       /* Capacity of the array                   */
    static seq_t last_scanned = 0;     /* Highest number already examined          */

    if (n == 0)                         /* Undefined for n = 0, harmless          */
        return 0;

    /* Return from cache if already computed */
    if (terms_len >= (size_t)n)
        return terms[(size_t)n - 1];

    /* Ensure the array can hold the required number of terms */
    ensure_capacity(&terms, &terms_cap, (size_t)n);

    /* Scan numbers higher than the last one examined until we reach n terms */
    for (seq_t num = last_scanned + 1;; ++num)
    {
        unsigned int cnt1 = 0, cnt3 = 0;
        seq_t tmp = num;
        /* Count digits 1 and 3 in the base‑5 representation */
        while (tmp > 0)
        {
            seq_t digit = tmp % 5;
            if (digit == 1)
                ++cnt1;
            else if (digit == 3)
                ++cnt3;
            tmp /= 5;
        }
        if (cnt1 == 3 && cnt3 == 2)
        {
            terms[terms_len++] = num;
            if (terms_len == (size_t)n)
            {
                last_scanned = num;
                return terms[(size_t)n - 1];
            }
        }
    }
}
