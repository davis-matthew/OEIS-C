/*
 * A035515  Zeckendorf expansion of n: repeatedly subtract the largest
 *          Fibonacci number you can until nothing remains. Little-endian
 *          concatenation of decimals.
 * https://oeis.org/A035515
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) is the little‑endian decimal concatenation of the
 *            Zeckendorf decomposition of n
 * Range:     u8 n <= 15, u16 n <= 50, u32 n <= 372, u64 n <= 6352,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 66 terms through n = 9624 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

/* Count decimal digits of a non‑negative idx_t value. */
static int digit_count(idx_t v)
{
    int d = 0;
    do {
        d++;
        v /= 10;
    } while (v);
    return d;
}

/* Return the a(n) term of OEIS A035515. */
seq_t a(idx_t n)
{
    if (n == 0)                /* a(0) = 0 */
        return (seq_t)0;

    /* Build Fibonacci numbers 1,2,3,5,... up to n. */
    idx_t fib[64];
    size_t fib_cnt = 0;
    fib[fib_cnt++] = 1;
    if (n >= 2)
        fib[fib_cnt++] = 2;

    while (1) {
        if (fib_cnt < 2) break;
        idx_t prev = fib[fib_cnt - 2];
        idx_t curr = fib[fib_cnt - 1];
        idx_t next = prev + curr;
        if (next <= n)
            fib[fib_cnt++] = next;
        else
            break;
    }

    /* Greedy Zeckendorf decomposition. */
    idx_t rem = n;
    idx_t chosen[64];
    size_t chosen_cnt = 0;
    idx_t i = fib_cnt;
    while (i--) {                 /* i goes from fib_cnt-1 down to 0 */
        if (fib[(size_t)i] <= rem) {
            rem -= fib[(size_t)i];
            chosen[chosen_cnt++] = fib[(size_t)i];
        }
    }

    /* Little‑endian concatenation of decimal representations. */
    seq_t result = 0;
    idx_t j = chosen_cnt;
    while (j--) {                 /* j goes from chosen_cnt-1 down to 0 */
        int d = digit_count(chosen[j]);
        seq_t p10 = pow10((idx_t)d);
        result = result * p10 + (seq_t)chosen[j];
    }

    return result;
}
