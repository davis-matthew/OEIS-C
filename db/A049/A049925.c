/*
 * A049925  a(n) = a(1) + a(2) + ... + a(n-1) - a(m) for n >= 4, where m =
 *          2^(p+1) + 2 - n and p is the unique integer such that 2^p < n - 1
 *          <= 2^(p+1), starting with a(1) = 1 and a(2) = a(3) = 3.
 * https://oeis.org/A049925
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  
 *  A049925
 *  Recurrence:
 *      a(1) = 1,  a(2) = a(3) = 3
 *      for n ≥ 4:
 *          let p = ⌊log₂(n‑2)⌋   (so 2^p < n‑1 ≤ 2^(p+1))
 *          let m = 2^(p+1) + 2 – n
 *          a(n) = (a(1)+…+a(n‑1)) – a(m)
 *  Prefix sums are kept to evaluate the recurrence in O(1) per term.
 */

#include <stdlib.h>
#include <stddef.h>

/* static storage for the sequence and its prefix sums */
static seq_t *a_arr   = NULL;   /* a_arr[i] = a(i)   (1‑based)   */
static seq_t *pref    = NULL;   /* pref[i] = a(1)+…+a(i) */
static size_t  cap    = 0;      /* allocated capacity ≥ n+1      */
static idx_t   maxn   = 0;      /* largest n computed          */

/* Ensure we have storage for indices up to n (1‑based). */
static void ensure_capacity(idx_t n)
{
    if (cap >= (size_t)(n + 1))
        return;

    size_t new_cap = cap ? cap << 1 : 16;
    while (new_cap < (size_t)(n + 1))
        new_cap <<= 1;

    /* realloc may return NULL on failure – let it propagate to a crash. */
    a_arr = (seq_t *)realloc(a_arr, new_cap * sizeof(seq_t));
    pref  = (seq_t *)realloc(pref,  new_cap * sizeof(seq_t));

    /* initialise pref[0] on first allocation */
    if (cap == 0)
        pref[0] = (seq_t)0;

    cap = new_cap;
}

/* floor(log₂(x)) for x ≥ 1  (x is an unsigned 64‑bit index). */
static unsigned int floor_log2(idx_t x)
{
    unsigned int p = 0;
    while (x > 1) {
        x >>= 1;
        ++p;
    }
    return p;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)            /* undefined, but guard against underflow */
        return (seq_t)0;

    ensure_capacity(n);

    if (n > maxn) {
        for (idx_t i = maxn + 1; i <= n; ++i) {
            seq_t val;
            if (i == 1)
                val = (seq_t)1;
            else if (i == 2 || i == 3)
                val = (seq_t)3;
            else {
                unsigned int p = floor_log2(i - 2);            /* 2^p < i‑1 ≤ 2^(p+1) */
                idx_t m = (((idx_t)1 << (p + 1)) + 2) - i;    /* 1‑based index */
                val = pref[i - 1] - a_arr[m];
            }
            a_arr[i] = val;
            pref[i]  = pref[i - 1] + val;
        }
        maxn = n;
    }
    return a_arr[n];
}
