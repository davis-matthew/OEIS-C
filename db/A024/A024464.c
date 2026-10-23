/*
 * A024464  s(1)t(n) + s(2)t(n-1) + ... + s(k)t(n+1-k), where k = [ (n+1)/2 ],
 *          s = (Fibonacci numbers), t = A000201 (lower Wythoff sequence).
 * https://oeis.org/A024464
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{i=1}^{⌊(n+1)/2⌋} F(i) · W(n+1-i) where F(i) is the
 *            i‑th Fibonacci number (F(1)=1,F(2)=1) and W(k)=⌊k·φ⌋ is the
 *            lower Wythoff (Beatty) sequence, φ=(1+√5)/2
 * Range:     u8 n <= 11, u16 n <= 30, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

/* dynamic cache for Fibonacci numbers */
static seq_t *fib_cache = NULL;      /* index 0..len-1 */
static size_t fib_len  = 0;

/* dynamic cache for lower Wythoff sequence */
static seq_t *tyw_cache = NULL;      /* index 0..len-1 */
static size_t tyw_len  = 0;

/* ensure fib_cache has entries 0..idx */
static void ensure_fib(size_t idx)
{
    if (fib_len > idx) return;

    size_t new_len = idx + 1;
    seq_t *new_cache = (seq_t*)realloc(fib_cache, new_len * sizeof(seq_t));
    if (!new_cache) { exit(EXIT_FAILURE); }
    fib_cache = new_cache;

    for (size_t i = fib_len; i <= idx; ++i) {
        if (i == 0)          fib_cache[i] = 0;
        else if (i == 1)     fib_cache[i] = 1;
        else                 fib_cache[i] = fib_cache[i-1] + fib_cache[i-2];
    }
    fib_len = new_len;
}

/* ensure tyw_cache has entries 0..idx */
static void ensure_wythoff(size_t idx)
{
    if (tyw_len > idx) return;

    size_t new_len = idx + 1;
    seq_t *new_cache = (seq_t*)realloc(tyw_cache, new_len * sizeof(seq_t));
    if (!new_cache) { exit(EXIT_FAILURE); }
    tyw_cache = new_cache;

    static const long double phi = (1.0L + sqrtl(5.0L)) / 2.0L;
    for (size_t i = tyw_len; i <= idx; ++i) {
        if (i == 0) continue;          /* index 0 unused */
        long double v = (long double)i * phi;
        unsigned long long val = (unsigned long long)floorl(v);
        tyw_cache[i] = (seq_t)val;
    }
    tyw_len = new_len;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    size_t us_n = (size_t)n;
    size_t k = (us_n + 1) >> 1;          /* floor((n+1)/2) */

    ensure_fib(k);
    ensure_wythoff(us_n);

    seq_t sum = 0;
    for (size_t i = 1; i <= k; ++i) {
        seq_t f = fib_cache[i];
        seq_t t = tyw_cache[us_n + 1 - i];
        sum += f * t;
    }
    return sum;
}
