/*
 * A024591  a(n) = s(1)s(n) + s(2)s(n-1) + ... + s(k)s(n+1-k), where k = [
 *          (n+1)/2 ], s = (F(2), F(3), ...).
 * https://oeis.org/A024591
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑ i=1..⌈(n+1)/2⌉ F(i+1) * F(n+2−i) where F(k) is the
 *            Fibonacci sequence with F(0)=0, F(1)=1
 * Range:     u8 n <= 8, u16 n <= 18, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* ---------- static storage ---------- */
static seq_t *fib      = NULL;   /* Fibonacci numbers F(0)... */
static seq_t *seqvals  = NULL;   /* sequence values 1‑based */
static size_t fib_cap  = 0, fib_sz = 0;
static size_t a_cap    = 0, a_sz   = 0;
static idx_t  computed_max_n = 0;

/* ---------- grow an array to at least 'need' elements ---------- */
static void grow_array(seq_t **arr, size_t *cap, size_t need)
{
    if (need <= *cap) return;
    size_t new_cap = (*cap == 0) ? 8 : (*cap << 1);
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t*)realloc(*arr, new_cap * sizeof(seq_t));
    if (!tmp) abort();            /* out of memory */
    *arr = tmp;
    *cap = new_cap;
}

/* ---------- ensure Fibonacci numbers up to index n+1 are ready ---------- */
static void ensure_fibonacci(idx_t n)
{
    /* Need indices 0 .. n+1 inclusive, so array size at least n+2. */
    size_t need = (size_t)n + 2;
    grow_array(&fib, &fib_cap, need);

    if (fib_sz == 0) {
        fib[0] = 0;
        fib[1] = 1;
        fib_sz = 2;
    }
    while (fib_sz <= (size_t)n + 1) {
        fib[fib_sz] = fib[fib_sz - 1] + fib[fib_sz - 2];
        ++fib_sz;
    }
}

/* ---------- ensure sequence values up to n are computed ---------- */
static void ensure_a(idx_t n)
{
    if (computed_max_n >= n) return;   /* already up to date */

    /* Ensure required Fibonacci numbers exist. */
    ensure_fibonacci(n);

    /* Make sure we have room for seqvals[1..n]. */
    size_t need = (size_t)n + 1;        /* seqvals[0] unused */
    grow_array(&seqvals, &a_cap, need);
    a_sz = need; /* keep size consistent even if unused */

    for (idx_t cur = computed_max_n + 1; cur <= n; ++cur) {
        idx_t k   = (cur + 1) / 2;
        seq_t sum = 0;
        for (idx_t i = 1; i <= k; ++i) {
            seq_t prod = fib[i + 1] * fib[cur + 2 - i];
            sum = sum + prod;
        }
        seqvals[cur] = sum;
    }
    computed_max_n = n;
}

/* ---------- public function ---------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;        /* undefined for n < 1 */
    ensure_a(n);
    return seqvals[n];
}
