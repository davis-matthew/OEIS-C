/*
 * A022953  a(n) = a(n-1) + c(n+1) for n >= 3, a( ) increasing, given a(1)=1,
 *          a(2)=7; where c( ) is complement of a( ).
 * https://oeis.org/A022953
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + c(n+1) (n ≥ 3) * where a(1)=1, a(2)=7 and c() is
 *            the complement of a() * (the n‑th positive integer that is not
 *            in the sequence a)
 * Range:     u8 n <= 18, u16 n <= 346, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* --- storage for the generated terms of a --- */
static seq_t *a_vals = NULL;      /* a_vals[1..] (index 0 unused) */
static size_t a_cap = 0;
static idx_t a_len = 0;           /* number of a-values computed */

/* --- storage for the values already in the sequence --- */
static uint64_t *used_vals = NULL;/* used_vals[0..used_len-1] (sorted) */
static size_t used_cap = 0;
static size_t used_len = 0;

/* Ensure a_vals has space for indices up to need-1 */
static void ensure_a_capacity(size_t need)
{
    if (a_cap >= need) return;
    size_t new_cap = a_cap ? a_cap << 1 : 16;
    while (new_cap < need) new_cap <<= 1;
    a_vals = (seq_t *)realloc(a_vals, new_cap * sizeof(seq_t));
    a_cap = new_cap;
}

/* Ensure used_vals has space for used_len+1 entries */
static void ensure_used_capacity(size_t need)
{
    if (used_cap >= need) return;
    size_t new_cap = used_cap ? used_cap << 1 : 16;
    while (new_cap < need) new_cap <<= 1;
    used_vals = (uint64_t *)realloc(used_vals, new_cap * sizeof(uint64_t));
    used_cap = new_cap;
}

/* Append a new used value; values are strictly increasing. */
static void append_used(uint64_t v)
{
    ensure_used_capacity(used_len + 1);
    used_vals[used_len++] = v;
}

/* Count how many used values are ≤ x (binary search). */
static inline size_t count_used_leq(uint64_t x)
{
    size_t lo = 0, hi = used_len;        /* hi is exclusive */
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (used_vals[mid] <= x) lo = mid + 1;
        else                      hi = mid;
    }
    return lo;                          /* number of used values ≤ x */
}

/* k‑th missing number (k ≥ 1 in the complement set). */
static inline seq_t kth_missing(idx_t k)
{
    uint64_t low  = 1;
    /* high guaranteed large enough: max candidate ≤ used_len + k + 1 */
    uint64_t high = (uint64_t)used_len + (uint64_t)k + 1;
    while (low < high) {
        uint64_t mid     = low + ((high - low) >> 1);
        uint64_t missing = mid - count_used_leq(mid);
        if (missing >= (uint64_t)k) high = mid;
        else                        low  = mid + 1;
    }
    return (seq_t)low;
}

/* Main routine: compute a(n) for any n ≥ 1, using memoization. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                /* undefined for n = 0 */
    if (n <= a_len) return a_vals[n];   /* already computed */

    ensure_a_capacity(n + 1);           /* indices up to n inclusive */

    for (idx_t i = a_len + 1; i <= n; ++i) {
        seq_t val;
        if (i == 1)          val = (seq_t)1;
        else if (i == 2)     val = (seq_t)7;
        else
        {
            seq_t diff = kth_missing(i + 1);      /* c(i+1) */
            val = a_vals[i - 1] + diff;           /* a(i-1)+c(i+1) */
        }
        a_vals[i] = val;
        append_used((uint64_t)val);
    }

    a_len = n;
    return a_vals[n];
}
