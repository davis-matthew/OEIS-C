/*
 * A118904  Perimeters of rectangles with integer sides and diagonal.
 * https://oeis.org/A118904
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A118904: Perimeters of rectangles with integer sides and diagonal.
   Formula: the sequence of all numbers of the form 2·(a+b) where a,b are
   integer solutions of a² + b² = c².  It is sorted in increasing order
   with duplicates removed. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Forward declarations of helper functions we can use. */
static idx_t gcd_idx_t(idx_t a, idx_t b);
static idx_t _floor_isqrt_u64(idx_t n);
static int cmp_func(const void *p1, const void *p2);

/* ---------- Simple integer helpers ---------- */
static idx_t gcd_idx_t(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

static idx_t _floor_isqrt_u64(idx_t v)
{
    /* Binary search for floor(sqrt(v)) using only integer ops. */
    idx_t low = 0, high = (idx_t)1 << 32;
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if (mid > 0 && mid > v / mid)
            high = mid - 1;
        else
            low = mid;
    }
    return low;
}

/* ---------- Comparator for qsort ---------- */
static int cmp_func(const void *p1, const void *p2)
{
    const seq_t *x = (const seq_t *)p1;
    const seq_t *y = (const seq_t *)p2;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

/* ---------- Dynamic array grow helper ---------- */
static void grow_array(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap) ? (*cap) * 2 : 16;
        if (new_cap < need) new_cap = need;
        seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!tmp && new_cap > 0) abort();  /* allocation failed */
        if (*arr) {
            for (size_t i = 0; i < *cap; ++i) tmp[i] = (*arr)[i];
            free(*arr);
        }
        *arr = tmp;
        *cap = new_cap;
    }
}

/* ---------- Cache of the sequence ---------- */
static seq_t *cached_perims = NULL;
static size_t cached_len   = 0;
static bool   cached_ready = false;

/* ---------- Precompute perimeters up to a chosen limit ---------- */
static void compute_perimeters(void)
{
    /* Upper bound that comfortably contains ≥10 000 terms.
       500 000 perimeters produce at least that many for this sequence.  */
    const idx_t LIMIT = 500000ULL;
    const idx_t MAX_M = _floor_isqrt_u64(LIMIT / 2) + 1;

    seq_t *arr = NULL;
    size_t cap = 0;
    size_t len = 0;

    for (idx_t m = 2; m <= MAX_M; ++m) {
        idx_t m2 = m * m;
        for (idx_t n = 1; n < m; ++n) {
            if (((m + n) & 1) == 0) continue;           /* one odd, one even */
            if (gcd_idx_t(m, n) != 1) continue;         /* coprime */

            /* Primitive Pythagorean pair (a,b).  Smallest side first. */
            idx_t a = m2 - n * n;
            idx_t b = 2 * m * n;
            if (a > b) { idx_t t = a; a = b; b = t; }

            idx_t perim = 2 * (a + b);
            if (perim > LIMIT) continue;

            /* Store primitive perimeter. */
            grow_array(&arr, &cap, len + 1);
            arr[len++] = (seq_t)perim;

            /* Store all multiples of the primitive perimeter. */
            for (seq_t k = 2; k * perim <= LIMIT; ++k) {
                grow_array(&arr, &cap, len + 1);
                arr[len++] = (seq_t)(k * perim);
            }
        }
    }

    if (len == 0) return;                 /* unlikely with LIMIT chosen */

    /* Sort and deduplicate. */
    qsort(arr, len, sizeof(seq_t), cmp_func);

    size_t uniq = 0;
    for (size_t i = 0; i < len; ++i) {
        if (i == 0 || arr[i] != arr[i - 1]) {
            arr[uniq++] = arr[i];
        }
    }
    len = uniq;

    /* Resize to the exact size. */
    seq_t *tmp = (seq_t *)realloc(arr, len * sizeof(seq_t));
    if (tmp) arr = tmp;

    cached_perims = arr;
    cached_len   = len;
    cached_ready = true;
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    if (!cached_ready) compute_perimeters();

    if (n == 0 || n > cached_len) return (seq_t)0; /* safety for out‑of‑range */
    return cached_perims[n - 1];
}
