/*
 * A052236  Primes p from A031924 such that A052180(primepi(p)) = 29.
 * https://oeis.org/A052236
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search: a(n) = n-th prime p with nextprime(p) = p+6 such that
 *            max(lpf(p+1),...,lpf(p+5)) = 29, i.e. A052180(primepi(p)) = 29
 * Range:     u8 none, u16 n <= 25, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Validated: partial, pass, 34 terms through n = 34 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t *A052236_terms = NULL;
static size_t A052236_cnt = 0, A052236_cap = 0;
static uint64_t A052236_next = 5;

static bool A052236_isprime(uint64_t m)
{
    if (m < 2)
        return false;
    if (m % 2 == 0)
        return m == 2;
    if (m % 3 == 0)
        return m == 3;
    for (uint64_t d = 5; d * d <= m; d += 6) {
        if (m % d == 0 || m % (d + 2) == 0)
            return false;
    }
    return true;
}

/* returns 0 if lpf > 29 (or m is a small prime), otherwise lpf (<=29) */
static unsigned A052236_small_lpf(uint64_t m)
{
    static const unsigned q[10] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29};
    for (int i = 0; i < 10; ++i) {
        if (m % q[i] == 0)
            return (m == q[i]) ? 0 : q[i];
    }
    return 0;
}

static bool A052236_ok(uint64_t p)
{
    bool has29 = false;
    for (uint64_t d = 1; d <= 5; ++d) {
        unsigned l = A052236_small_lpf(p + d);
        if (l == 0)
            return false;
        if (l == 29)
            has29 = true;
    }
    if (!has29)
        return false;
    /* p+1..p+5 are all composite (each has a proper factor <= 29) */
    return A052236_isprime(p) && A052236_isprime(p + 6);
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;
    while (A052236_cnt < n) {
        uint64_t p = A052236_next;
        A052236_next += 2;
        if (A052236_ok(p)) {
            if (A052236_cnt == A052236_cap) {
                A052236_cap = A052236_cap ? A052236_cap * 2 : 64;
                A052236_terms = (uint64_t *)realloc((void *)A052236_terms,
                                                    A052236_cap * sizeof(uint64_t));
                if (!A052236_terms)
                    abort();
            }
            A052236_terms[A052236_cnt++] = p;
        }
    }
    return (seq_t)A052236_terms[n - 1];
}
