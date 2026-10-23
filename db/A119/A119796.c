/*
 * A119796  Zero through ten in alphabetical order of English reverse
 *          spelling.
 * https://oeis.org/A119796
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, easy, nonn, word
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 11, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for A119796: 0..10 sorted by alphabetical
   order of their reversed English names. The sequence is
   periodic of length 11. */

#include <stdbool.h>
#include <string.h>

/* The English words for 0‐10. */
static const char *NUM_WORDS[11] =
{
    "zero","one","two","three","four","five",
    "six","seven","eight","nine","ten"
};

/* Compare two numbers by the alphabetic order of the
   reversed spelling of their words. */
static int cmp_rev(idx_t a, idx_t b)
{
    const char *wa = NUM_WORDS[a];
    const char *wb = NUM_WORDS[b];
    int ia = (int)strlen(wa) - 1;
    int ib = (int)strlen(wb) - 1;
    while (ia >= 0 && ib >= 0)
    {
        char ca = wa[ia];
        char cb = wb[ib];
        if (ca < cb) return -1;
        if (ca > cb) return  1;
        --ia;
        --ib;
    }
    if (ia < 0 && ib < 0) return 0;
    return (ia < 0) ? -1 : 1;
}

/* Insertion sort of an array of idx_t using cmp_rev. */
static void sort_idx(idx_t *arr, size_t len)
{
    for (size_t i = 1; i < len; ++i)
    {
        idx_t key = arr[i];
        size_t j = i;
        while (j > 0 && cmp_rev(key, arr[j-1]) < 0)
        {
            arr[j] = arr[j-1];
            --j;
        }
        arr[j] = key;
    }
}

/* Cache of the sorted numbers (periodic part). */
static seq_t CACHE[11];
static bool INIT = false;

seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* offset is 1 */

    /* lazy initialization – build the cached list once. */
    if (!INIT)
    {
        idx_t tmp[11];
        for (idx_t i = 0; i < 11; ++i) tmp[i] = i;
        sort_idx(tmp, 11);
        for (size_t i = 0; i < 11; ++i) CACHE[i] = (seq_t)tmp[i];
        INIT = true;
    }

    /* periodic sequence: repeat every 11 terms. */
    size_t pos = (size_t)((n-1) % 11);
    return CACHE[pos];
}
