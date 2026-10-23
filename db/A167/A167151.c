/*
 * A167151  a(2n+1) = a(2n) + a(2n-1), a(2n)=least number not yet in the
 *          sequence, a(1)=1.
 * https://oeis.org/A167151
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(2n+1) = a(2n) + a(2n-1), a(2n)=least non‑negative integer not
 *            yet in the sequence, a(0)=0, a(1)=1. The sequence is a(0)=0,
 *            a(1)=1, a(2)=2, a(3)=3, a(4)=4, a(5)=7, etc
 * Range:     u8 n <= 38, u16 n <= 694, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*----------------------------------------------------------------------------*/

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ---------- sequence storage ---------- */
static seq_t *seq = NULL;          /* array of sequence terms          */
static size_t seq_cap = 0;         /* current capacity of seq          */
static size_t seq_len = 0;         /* number of terms stored           */

/* ---------- used‑numbers storage (sorted array) ---------- */
static seq_t *used = NULL;         /* sorted array of already used values */
static size_t used_cap = 0;        /* capacity of used array             */
static size_t used_len = 0;        /* number of elements in used array   */

/* ---------- mex candidate ---------- */
static seq_t mex_candidate;        /* smallest non‑negative integer not yet used */

/* ---------- helper to grow the sequence array ---------- */
static void seq_expand(size_t needed)
{
    if (seq_cap >= needed) return;
    size_t newcap = seq_cap ? seq_cap << 1 : 64;
    while (newcap < needed) newcap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq, newcap * sizeof(seq_t));
    if (!tmp) return;            /* allocation failed – keep old array   */
    seq = tmp;
    seq_cap = newcap;
}

/* ---------- helper to grow the used array ---------- */
static void used_expand(size_t needed)
{
    if (used_cap >= needed) return;
    size_t newcap = used_cap ? used_cap << 1 : 8;
    while (newcap < needed) newcap <<= 1;
    seq_t *tmp = (seq_t *)realloc(used, newcap * sizeof(seq_t));
    if (!tmp) return;            /* allocation failed – keep old array   */
    used = tmp;
    used_cap = newcap;
}

/* ---------- binary search: does 'val' appear in 'used'? ---------- */
static bool used_contains(const seq_t *val)
{
    size_t lo = 0, hi = used_len;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (used[mid] < *val) lo = mid + 1;
        else                  hi = mid;
    }
    return (lo < used_len && used[lo] == *val);
}

/* ---------- binary search insert: add 'val' to sorted 'used' ---------- */
static void used_insert(const seq_t val)
{
    size_t lo = 0, hi = used_len;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (used[mid] < val) lo = mid + 1;
        else                 hi = mid;
    }
    /* already present – nothing to do */
    if (lo < used_len && used[lo] == val) return;

    used_expand(used_len + 1);
    for (size_t i = used_len; i > lo; --i) used[i] = used[i - 1];
    used[lo] = val;
    used_len++;
}

/* ---------- initialise everything on first call ---------- */
static void seq_init(void)
{
    /* initialise sequence array and first two terms */
    seq_expand(2);
    seq[0] = (seq_t)0;
    seq[1] = (seq_t)1;
    seq_len = 2;

    /* initialise used array with 0 and 1 */
    used_expand(2);
    used[0] = (seq_t)0;
    used[1] = (seq_t)1;
    used_len = 2;

    /* first unused candidate after 0 and 1 is 2 */
    mex_candidate = (seq_t)2;
}

/* ---------- the required function ---------- */
seq_t a(idx_t n)
{
    if (seq_len == 0) seq_init();          /* initialise once */

    if ((idx_t)n < (idx_t)seq_len) return seq[n];

    /* extend sequence up to n */
    seq_expand((size_t)n + 1);
    for (size_t i = seq_len; i <= (size_t)n; ++i) {
        if ((i & 1) == 0) {                /* even index : mex */
            seq_t val = mex_candidate;
            while (used_contains(&val)) val++;   /* search next unused */
            seq[i] = val;
            used_insert(val);
            mex_candidate = val + (seq_t)1;
        } else {                           /* odd index : sum of two previous */
            seq[i] = seq[i - 1] + seq[i - 2];
            used_insert(seq[i]);
        }
    }
    seq_len = (size_t)n + 1;
    return seq[n];
}
