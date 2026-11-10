/*
 * A072776  Exponents of powers of squarefree numbers.
 * https://oeis.org/A072776
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = e where the n-th term of A072774 (numbers m = r^e with r
 *            squarefree, i.e. all prime exponents equal) is m = A072775(n)^e;
 *            found by a sieve-free incremental search with trial division,
 *            and a(1) = 1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 121 terms through n = 9909 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static unsigned char *a072776_exps = NULL;
static size_t a072776_len = 0;
static size_t a072776_cap = 0;
static uint64_t a072776_next = 1;

/* returns common exponent if all prime exponents of m equal, else 0 */
static unsigned a072776_common_exp(uint64_t m)
{
    if (m == 1) return 1;
    unsigned first = 0;
    uint64_t p;
    for (p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            unsigned e = 0;
            while (m % p == 0) { m /= p; ++e; }
            if (first == 0) first = e;
            else if (e != first) return 0;
        }
    }
    if (m > 1) {
        if (first == 0) first = 1;
        else if (first != 1) return 0;
    }
    return first;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    while (a072776_len < (size_t)n) {
        unsigned e = a072776_common_exp(a072776_next);
        ++a072776_next;
        if (e) {
            if (a072776_len == a072776_cap) {
                a072776_cap = a072776_cap ? a072776_cap * 2 : 1024;
                a072776_exps = (unsigned char *)realloc(a072776_exps, a072776_cap);
            }
            a072776_exps[a072776_len++] = (unsigned char)e;
        }
    }
    return (seq_t)a072776_exps[n - 1];
}
