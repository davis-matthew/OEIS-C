/*
 * A158883  G.f. satisfies: [x^n] A(x)^(n+1) = [x^n] A(x)^n for n>1 with
 *          A(0)=A'(0)=1.
 * https://oeis.org/A158883
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 11, i64 n <= 19, i128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = -(n) * ( sum_{i=1}^{n-1} a_i a_{n-i} / (i*(n-i))
 *                        + sum_{i=1}^{n-2} (n-i-1) * a_i a_{n-i} / (i*(n-i)) )
 * with a(0)=1, a(1)=1.
 * This recurrence derives from the generating‑function equation
 * x²A′(x)=xA(x)+A(x)²−A(x)³ and holds for all n≥2.
 * The implementation uses O(n²) arithmetic and works with any
 * integer type seq_t supplied by the build.  For 64‑bit builds the
 * products are performed in __int128 to avoid signed overflow.
 */
 
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
 
/* helper to multiply a seq_t by a small idx_t (≤ n) safely */
static inline seq_t mul_int(seq_t x, idx_t y)
{
    if (sizeof(seq_t) <= 8) {
        __int128 r = (__int128)x * (__int128)y;
        return (seq_t)r;
    } else {
        return x * y;
    }
}
 
/* helper to compute (x*y)/d with exact division */
static inline seq_t mul_div(seq_t x, seq_t y, seq_t d)
{
    if (sizeof(seq_t) <= 8) {
        __int128 r = (__int128)x * (__int128)y;
        return (seq_t)(r / d);
    } else {
        return (x * y) / d;
    }
}
 
/* static cache for computed terms */
static seq_t *a_cache = NULL;
static idx_t a_len   = 0;   /* length of a_cache (number of terms) */
 
/* compute a_cache up to index n (inclusive) */
static void compute_a(idx_t n)
{
    if (a_len > n) return;          /* already computed enough */
 
    size_t new_len = (size_t)(n + 1);
    seq_t *new_arr = (seq_t*)malloc(new_len * sizeof(seq_t));
    if (!new_arr) return;           /* memory error → caller sees 0 */
 
    /* copy existing values if any */
    for (size_t i = 0; i < a_len && i < new_len; ++i) new_arr[i] = a_cache[i];
 
    /* initialise base cases */
    if (a_len == 0) {
        new_arr[0] = (seq_t)1;
        if (new_len > 1) new_arr[1] = (seq_t)1;
    } else if (a_len == 1 && new_len > 1) {
        new_arr[1] = (seq_t)1;
    }
 
    /* compute new terms */
    for (size_t k = a_len; k <= n; ++k) {
        if (k == 0) { new_arr[0] = 1; continue; }
        if (k == 1) { new_arr[1] = 1; continue; }
 
        seq_t sum1 = 0;
        seq_t sum2 = 0;
        for (size_t i = 1; i <= k - 1; ++i) {
            seq_t denom = (seq_t)(i * (k - i));
            seq_t prod  = mul_div(new_arr[i], new_arr[k - i], denom);
            sum1 += prod;
            if (i <= k - 2) {
                seq_t term = mul_int(prod, (idx_t)(k - i - 1));
                sum2 += term;
            }
        }
        seq_t total = sum1 + sum2;
        new_arr[k]  = - mul_int(total, (idx_t)k);
    }
 
    free(a_cache);
    a_cache = new_arr;
    a_len   = new_len;
}
 
/* public function implementing the OEIS sequence */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;     /* a(0)=1 */
    compute_a(n);
    return a_cache[n];
}
