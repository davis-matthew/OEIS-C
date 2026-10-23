/*
 * A024599  a(n) = s(1)t(n) + s(2)t(n-1) + ... + s(k)t(n+1-k), where k = [
 *          (n+1)/2 ], s = (odd natural numbers), t = A000201 (lower Wythoff
 *          sequence).
 * https://oeis.org/A024599
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{i=1}^{⌊(n+1)/2⌋} (2i-1) * floor( ((n+1-i) +
 *            floor(√(5*(n+1-i)²)) ) / 2 )
 * Range:     u8 n <= 9, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Integer square root: floor(√x) for 0 ≤ x < 2⁶⁴ */
static uint64_t floor_sqrt_u64(uint64_t x)
{
    uint64_t lo = 0, hi = 0xFFFFFFFFULL; /* high enough for 64‑bit numbers */
    while (lo <= hi) {
        uint64_t mid = lo + ((hi - lo) >> 1);
        uint64_t sq  = mid * mid;
        if (sq == x) return mid;
        if (sq < x)  lo = mid + 1;
        else         hi = mid - 1;
    }
    return hi; /* hi is floor sqrt(x) */
}

/* ---------- static caches ---------- */
static seq_t *Wythoff = NULL; /* t(n) = floor(n*φ) */
static idx_t  Wythoff_len = 0;
static seq_t *A_cache   = NULL; /* computed a(n) values */
static idx_t  A_cache_len = 0;

/* Compute lower Wythoff term t(n) using integer arithmetic */
static seq_t lower_wythoff(idx_t n)
{
    /* t(n) = floor((n + floor(√(5*n*n))) / 2) */
    uint64_t sq      = 5ULL * n * n;
    uint64_t sqrt_sq = floor_sqrt_u64(sq);
    return (seq_t)((n + sqrt_sq) / 2);
}

/* Ensure Wythoff cache contains values up to n (inclusive) */
static void ensure_wythoff(idx_t n)
{
    if (Wythoff_len >= n) return;
    size_t new_len = n + 1;
    seq_t *tmp = (seq_t*)realloc(Wythoff, new_len * sizeof(seq_t));
    if (!tmp) abort();
    Wythoff = tmp;
    for (idx_t i = Wythoff_len + 1; i <= n; ++i)
        Wythoff[i] = lower_wythoff(i);
    Wythoff_len = n;
}

/* Ensure a(n) cache contains values up to n (inclusive) */
static void ensure_a(idx_t n)
{
    if (A_cache_len >= n) return;
    size_t new_len = n + 1;
    seq_t *tmp = (seq_t*)realloc(A_cache, new_len * sizeof(seq_t));
    if (!tmp) abort();
    A_cache = tmp;

    for (idx_t i = A_cache_len + 1; i <= n; ++i) {
        idx_t k   = (i + 1) / 2;
        seq_t sum = 0;
        for (idx_t j = 1; j <= k; ++j) {
            seq_t t_val = Wythoff[i + 1 - j];
            seq_t s_val = (seq_t)(2 * j - 1);
            sum += s_val * t_val;
        }
        A_cache[i] = sum;
    }
    A_cache_len = n;
}

/* ----------------- required function ----------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* out of offset, not used */
    ensure_wythoff(n);
    ensure_a(n);
    return A_cache[n];
}
