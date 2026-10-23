/*
 * A043189  Numbers k such that 1 and 3 occur juxtaposed in the base-9
 *          representation of k but not of k-1.
 * https://oeis.org/A043189
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A043189
    Numbers k such that the base‑9 digits 1 and 3 occur adjacent
    in the representation of k but not in that of k‑1.
    Offset: 1
*/

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* helper: test whether any adjacent pair of digits in base‑9
   representation of x is (1,3) or (3,1) */
static bool has_adjacent_13(seq_t x)
{
    if (x == 0) return false;
    int prev = -1;
    seq_t tmp = x;
    while (tmp > 0) {
        int digit = (int)(tmp % 9);
        if (prev == 1 && digit == 3) return true;
        if (prev == 3 && digit == 1) return true;
        prev = digit;
        tmp /= 9;
    }
    return false;
}

/* dynamic array helpers */
static void ensure_capacity(seq_t **arr, size_t *cap, size_t needed)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap > 0) ? (*cap << 1) : 16;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *new_arr = (seq_t*)malloc(new_cap * sizeof(seq_t));
    if (*arr != NULL) {
        memcpy(new_arr, *arr, (*cap) * sizeof(seq_t));
        free(*arr);
    }
    *arr = new_arr;
    *cap = new_cap;
}

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;          /* array of terms */
    static size_t cache_size = 0;        /* number of terms stored */
    static size_t cache_cap = 0;         /* allocated capacity */
    static seq_t next_k = 1;             /* next integer to test */

    if (n == 0) return 0;  /* undefined for n==0, but return 0 */

    /* compute terms up to n */
    while (cache_size < n) {
        /* find next candidate */
        while (!has_adjacent_13(next_k) || has_adjacent_13(next_k - 1))
            next_k++;
        /* allocate space for new term */
        ensure_capacity(&cache, &cache_cap, cache_size + 1);
        cache[cache_size++] = next_k;
        next_k++;  /* advance for next search */
    }

    return cache[n - 1];
}
