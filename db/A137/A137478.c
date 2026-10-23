/*
 * A137478  A triangle of recursive Fibonacci Lah numbers: f(n) =
 *          Fibonacci(n)*f(n - 1), L(n, k) = binomial(n-1, k-1)*(f(n)/f(k)).
 * https://oeis.org/A137478
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = binomial(r-1, k-1) * f(r)/f(k) where r =
 *            ceil((sqrt(1+8n)-1)/2), k = n - r(r-1)/2, f(m) = ∏_{i=1..m}
 *            Fibonacci(i)
 * Range:     u8 n <= 16, u16 n <= 29, u32 n <= 55, u64 n <= 105,
 *            u128 n <= 192, big any n
 * Helpers:   binomial, fib, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/fib.h"
#include "../utils/binomial.h"
#include "../utils/seq_mul.h"

static seq_t *f = NULL;
static size_t f_cap = 0;

/* Ensure f array can hold indices up to n (inclusive) */
static void ensure_f_cap(idx_t n) {
    if (f_cap <= (size_t)n) {
        size_t new_cap = f_cap ? f_cap << 1 : 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        f = (seq_t *)realloc(f, new_cap * sizeof(seq_t));
        for (size_t i = f_cap; i < new_cap; ++i) f[i] = 0;
        f_cap = new_cap;
    }
}

/* Build f array up to r, computing f[i] = f[i-1] * Fibonacci(i) */
static void build_f_to(idx_t r) {
    ensure_f_cap(r);
    if (f[0] == 0) f[0] = 1;
    for (size_t i = 1; i <= (size_t)r; ++i) {
        if (f[i] == 0) {
            seq_t fib_i = fib((idx_t)i);
            f[i] = seq_mul(f[i-1], fib_i);
        }
    }
}

/* Find the row r such that r(r+1)/2 >= n */
static idx_t row_for_idx(idx_t n) {
    idx_t lo = 1, hi = 1;
    while (hi * (hi + 1) / 2 < n) hi <<= 1;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (mid * (mid + 1) / 2 >= n) hi = mid;
        else lo = mid + 1;
    }
    return lo;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;              /* undefined for n < 1 */
    idx_t r = row_for_idx(n);
    idx_t prev = (r - 1) * r / 2;
    idx_t k = n - prev;
    build_f_to(r);
    seq_t bin = binomial(r - 1, k - 1);
    seq_t f_div = f[r] / f[k];
    return seq_mul(bin, f_div);
}
