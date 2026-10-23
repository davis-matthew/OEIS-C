/*
 * A199631  Numbers having each digit once and whose cube has each digit three
 *          times.
 * https://oeis.org/A199631
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A199631: numbers having each digit once
   and whose cube has each digit three times.
   The implementation enumerates all 10‑digit pandigital numbers,
   filters those whose cube has 30 digits with exact frequencies of 3
   for each decimal digit, stores the qualifying numbers in a sorted
   cache, and returns the n‑th element (1‑based). */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ----- Helper: next_permutation ------------------------------------- */
static bool next_permutation(int *a, int n)
{
    int i = n - 2;
    while (i >= 0 && a[i] >= a[i + 1]) i--;
    if (i < 0) return false;
    int j = n - 1;
    while (a[j] <= a[i]) j--;
    int tmp = a[i];
    a[i] = a[j];
    a[j] = tmp;
    for (int l = i + 1, r = n - 1; l < r; l++, r--) {
        tmp = a[l];
        a[l] = a[r];
        a[r] = tmp;
    }
    return true;
}

/* ----- Helper: comparator for qsort of 64‑bit numbers -------------- */
static int cmp_uint64(const void *p1, const void *p2)
{
    const uint64_t *x = (const uint64_t *)p1;
    const uint64_t *y = (const uint64_t *)p2;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

/* ----- Static cache variables ---------------------------------------- */
static uint64_t *cache      = NULL;   /* sorted list of qualifying numbers   */
static size_t   cache_len   = 0;      /* number of entries in cache         */
static bool     cache_built = false; /* flag to build cache once            */

/* ----- Build the cache of all 10‑digit pandigital cubes -------------- */
static void build_cache(void)
{
    int digits[10] = {0,1,2,3,4,5,6,7,8,9};
    uint64_t *tmp_list = NULL;
    size_t tmp_cap = 0, tmp_len = 0;

    /* Iterate over all permutations of 0‑9 */
    do {
        if (digits[0] == 0) continue;          /* skip leading zero */

        /* Form the 10‑digit number */
        uint64_t num = 0;
        for (int i = 0; i < 10; ++i)
            num = num * 10 + (uint64_t)digits[i];

        /* Compute the cube in unsigned __int128 */
        unsigned __int128 cube = (unsigned __int128)num * num * num;

        /* Count digits of the cube */
        unsigned char freq[10] = {0};
        unsigned int total_digits = 0;
        unsigned __int128 tmp = cube;

        while (tmp > 0) {
            unsigned int d = (unsigned int)(tmp % 10);
            freq[d]++;
            total_digits++;
            tmp /= 10;
        }

        /* The cube must have exactly 30 digits, each appearing 3 times */
        if (total_digits != 30) continue;
        bool ok = true;
        for (int d = 0; d < 10; ++d) {
            if (freq[d] != 3) { ok = false; break; }
        }
        if (!ok) continue;

        /* Store the qualifying number */
        if (tmp_len == tmp_cap) {
            size_t new_cap = tmp_cap == 0 ? 64 : tmp_cap * 2;
            uint64_t *new_list = (uint64_t *)realloc(tmp_list, new_cap * sizeof(uint64_t));
            if (!new_list) { free(tmp_list); abort(); }
            tmp_list = new_list;
            tmp_cap = new_cap;
        }
        tmp_list[tmp_len++] = num;

    } while (next_permutation(digits, 10));

    /* Sort the list */
    qsort(tmp_list, tmp_len, sizeof(uint64_t), cmp_uint64);

    cache = tmp_list;
    cache_len = tmp_len;
    cache_built = true;
}

/* ----- Public function ------------------------------------------------ */
seq_t a(idx_t n)
{
    if (!cache_built) build_cache();

    if (n == 0) return (seq_t)0;                     /* undefined for n=0  */
    if ( (idx_t)(n-1) >= cache_len ) return (seq_t)0; /* n beyond known terms */

    uint64_t val = cache[n-1];
    return (seq_t)val;
}
