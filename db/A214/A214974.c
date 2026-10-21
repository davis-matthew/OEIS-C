/*
 * A214974  Numbers k for which A116543(k) = A007895(k); i.e., the Lucas and
 *          Zeckendorf representations of k have the same length.
 * https://oeis.org/A214974
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth positive integer k for which the Zeckendorf *
 *            representation of k and the Lucas representation of k contain
 *            the same * number of summands. The sequence is indexed with
 *            offset 1
 * Range:     u8 n <= 59, u16 n <= 8000, u32 n <= 8000, u64 n <= 8000,
 *            u128 n <= 8000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation builds Fibonacci and Lucas numbers on demand and
 * compares the greedy representation lengths.  Results are memoized
 * so that calls are O(1) after the first pass up to the desired n. */
#include <stdlib.h>

/* --- Fibonacci (Zeckendorf) helpers ---------------------------------------------------- */
static seq_t *fib_arr = NULL;
static size_t fib_cnt = 0, fib_cap = 0;

static void ensure_fib_cap(size_t needed)
{
    if (fib_cap < needed) {
        fib_cap = fib_cap ? fib_cap * 2 : 4;
        if (fib_cap < needed) fib_cap = needed;
        seq_t *tmp = (seq_t *)realloc(fib_arr, fib_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        fib_arr = tmp;
    }
}

static void generate_fibs_upto(seq_t limit)
{
    if (!fib_arr) {
        ensure_fib_cap(4);
        fib_arr[0] = (seq_t)1;
        fib_arr[1] = (seq_t)2;
        fib_cnt = 2;
    }
    while (fib_arr[fib_cnt - 1] < limit) {
        ensure_fib_cap(fib_cnt + 1);
        fib_arr[fib_cnt] = fib_arr[fib_cnt - 1] + fib_arr[fib_cnt - 2];
        ++fib_cnt;
    }
}

static size_t zeckendorf_len(seq_t n)
{
    generate_fibs_upto(n);
    size_t len = 0;
    size_t idx = fib_cnt - 1;
    while (n > 0) {
        while (fib_arr[idx] > n) --idx;
        n -= fib_arr[idx];
        ++len;
    }
    return len;
}

/* --- Lucas helpers -------------------------------------------------------------- */
static seq_t *luc_raw  = NULL;        /* natural order, not used for greedy */
static seq_t *luc_sorted = NULL;     /* ascending order for greedy */
static size_t luc_raw_cnt  = 0, luc_sorted_cnt = 0;
static size_t luc_raw_cap  = 0, luc_sorted_cap = 0;

static void ensure_luc_raw_cap(size_t needed)
{
    if (luc_raw_cap < needed) {
        luc_raw_cap = luc_raw_cap ? luc_raw_cap * 2 : 4;
        if (luc_raw_cap < needed) luc_raw_cap = needed;
        seq_t *tmp = (seq_t *)realloc(luc_raw, luc_raw_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        luc_raw = tmp;
    }
}

static void ensure_luc_sorted_cap(size_t needed)
{
    if (luc_sorted_cap < needed) {
        luc_sorted_cap = luc_sorted_cap ? luc_sorted_cap * 2 : 4;
        if (luc_sorted_cap < needed) luc_sorted_cap = needed;
        seq_t *tmp = (seq_t *)realloc(luc_sorted, luc_sorted_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        luc_sorted = tmp;
    }
}

/* insert value into luc_sorted preserving ascending order */
static void luc_insert_sorted(seq_t val)
{
    size_t lo = 0, hi = luc_sorted_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (luc_sorted[mid] < val) lo = mid + 1;
        else hi = mid;
    }
    ensure_luc_sorted_cap(luc_sorted_cnt + 1);
    for (size_t i = luc_sorted_cnt; i > lo; --i) luc_sorted[i] = luc_sorted[i-1];
    luc_sorted[lo] = val;
    ++luc_sorted_cnt;
}

static void generate_lucas_upto(seq_t limit)
{
    if (!luc_raw) {
        ensure_luc_raw_cap(4);
        luc_insert_sorted((seq_t)2);  /* L0 = 2 */
        luc_insert_sorted((seq_t)1);  /* L1 = 1 */
        ensure_luc_raw_cap(2);
        luc_raw[0] = (seq_t)2;
        luc_raw[1] = (seq_t)1;
        luc_raw_cnt = 2;
    }
    while (luc_sorted_cnt == 0 || luc_sorted[luc_sorted_cnt-1] < limit) {
        ensure_luc_raw_cap(luc_raw_cnt + 1);
        seq_t next = luc_raw[luc_raw_cnt-1] + luc_raw[luc_raw_cnt-2];
        luc_raw[luc_raw_cnt] = next;
        ++luc_raw_cnt;
        luc_insert_sorted(next);
    }
}

static size_t lucas_len(seq_t n)
{
    generate_lucas_upto(n);
    size_t len = 0;
    size_t idx = luc_sorted_cnt - 1;
    while (n > 0) {
        while (luc_sorted[idx] > n) --idx;
        n -= luc_sorted[idx];
        ++len;
    }
    return len;
}

/* --- Sequence storage ------------------------------------------------------------ */
static seq_t *seq_vals = NULL;
static size_t seq_len = 0, seq_cap = 0;

static void seq_push(seq_t val)
{
    if (seq_len >= seq_cap) {
        seq_cap = seq_cap ? seq_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(seq_vals, seq_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        seq_vals = tmp;
    }
    seq_vals[seq_len++] = val;
}

/* --- Result function ------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset starts at 1 */
    static idx_t next_k = 1;                 /* next candidate to test */
    while (seq_len < (size_t)n) {
        seq_t k = (seq_t)next_k;
        if (zeckendorf_len(k) == lucas_len(k))
            seq_push(k);
        ++next_k;
    }
    return seq_vals[(size_t)n - 1];
}
