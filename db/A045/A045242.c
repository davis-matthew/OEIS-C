/*
 * A045242  Numbers whose base-5 representation contains exactly two 1's and
 *          two 3's.
 * https://oeis.org/A045242
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers whose base‑5 representation contains exactly two 1's and
 *            two 3's
 * Range:     u8 none, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for any n >= 1, memoizing previously computed terms. */

#include <stdlib.h>

/* internal cache for memoization */
static seq_t  *cache   = NULL;   /* array of computed terms */
static size_t  cache_len  = 0;   /* number of terms currently cached */
static size_t  cache_cap  = 0;   /* allocated capacity of cache */
static idx_t   next_len   = 4;   /* next base‑5 length to generate */

/*------------- helper -----------------------------------------------------*/
static void ensure_capacity(seq_t **arr, size_t *len, size_t *cap, size_t add)
{
    while (*len + add > *cap) {
        size_t new_cap = (*cap == 0) ? 16 : (*cap << 1);
        *arr = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        *cap = new_cap;
    }
}

static void cache_push(seq_t val)
{
    ensure_capacity(&cache, &cache_len, &cache_cap, 1);
    cache[cache_len++] = val;
}

static int cmp_seq(const void *a, const void *b)
{
    const seq_t *x = (const seq_t *)a;
    const seq_t *y = (const seq_t *)b;
    if (*x < *y) return -1;
    if (*x > *y) return  1;
    return 0;
}

/* recursive enumeration of all length‑L base‑5 numbers
   that contain exactly two 1's and two 3's, first digit non‑zero   */
static void enumerate_len(idx_t L, idx_t pos, seq_t val, int rem1, int rem3)
{
    if (pos == L) {               /* all digits chosen */
        if (rem1 == 0 && rem3 == 0)
            cache_push(val);
        return;
    }
    int slots_left = L - pos - 1;

    for (int d = 0; d <= 4; ++d) {
        if (pos == 0 && d == 0) continue;   /* leading zero not allowed */

        if (d == 1) {
            if (rem1 == 0) continue;
            int need = rem1 - 1 + rem3;
            if (need > slots_left) continue;
            enumerate_len(L, pos + 1, val * 5 + d, rem1 - 1, rem3);
        }
        else if (d == 3) {
            if (rem3 == 0) continue;
            int need = rem1 + rem3 - 1;
            if (need > slots_left) continue;
            enumerate_len(L, pos + 1, val * 5 + d, rem1, rem3 - 1);
        }
        else { /* d in {0,2,4} */
            int need = rem1 + rem3;
            if (need > slots_left) continue;
            enumerate_len(L, pos + 1, val * 5 + d, rem1, rem3);
        }
    }
}

/* public function --------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* out of range, safe default */

    /* ensure enough terms are cached */
    while (cache_len < (size_t)n) {
        enumerate_len(next_len, 0, 0, 2, 2);   /* generate all of this length */
        qsort(cache, cache_len, sizeof(seq_t), cmp_seq);  /* keep sorted */
        ++next_len;                     /* next base‑5 length */
    }
    return cache[n - 1];               /* a(1) corresponds to cache[0] */
}
