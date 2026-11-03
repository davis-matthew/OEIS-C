/*
 * A122633  F(n)_n where F() = Fibonacci numbers A000045.
 * https://oeis.org/A122633
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = interpret the decimal string of the n‑th Fibonacci number
 *            F(n) (OEIS A000045) as a number written in base n. In this way
 *            a(n) = Σ_{k=0}^{m-1} dₖ · n^k where dₖ are the decimal digits of
 *            F(n)
 * Range:     u8 n <= 12, u16 n <= 20, u32 n <= 33, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Validated: partial, pass, 36 terms through n = 36 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

static seq_t fib(idx_t n) {
    if (n <= 2) return (seq_t)1;
    seq_t f1 = 1, f2 = 1, f3;
    for (idx_t i = 3; i <= n; ++i) {
        f3 = f1 + f2;
        f1 = f2;
        f2 = f3;
    }
    return f2;
}

/* convert a seq_t value to a decimal string */
static void seq_to_str(seq_t v, char *buf, size_t *len)
{
    if (v == 0) {
        buf[0] = '0';
        *len = 1;
        return;
    }
    char tmp[64];
    int pos = 0;
    seq_t ten = (seq_t)10;
    while (v != 0) {
        tmp[pos++] = '0' + (int)(v % ten);
        v /= ten;
    }
    for (int i = 0; i < pos; ++i)
        buf[i] = tmp[pos - 1 - i];
    *len = pos;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset is 1; this case is irrelevant */
    seq_t fn = fib(n);                /* F(n) */
    /* buffer big enough for F(10000) which has ~2089 decimal digits */
    char buf[4000];
    size_t len;
    seq_to_str(fn, buf, &len);

    seq_t result = 0;
    seq_t base   = (seq_t)n;
    for (size_t i = 0; i < len; ++i) {
        result = result * base + (seq_t)(buf[i] - '0');
    }
    return result;
}
