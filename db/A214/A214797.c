/*
 * A214797  Numbers n such that n+10=A214795(n).
 * https://oeis.org/A214797
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th number m with A214795(m) = m+10, i.e. the smallest
 *            k>=2 with m | Fibonacci(k-1)+21 is k = m+10; found by search
 *            (filter F(m+9)+21 == 0 mod m via fast doubling, then confirm no
 *            smaller index by linear scan)
 * Range:     u8 n <= 12, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Validated: partial, pass, 42 terms through n = 42 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t A214797_mm(uint64_t a, uint64_t b, uint64_t m)
{
    return (uint64_t)(((unsigned __int128)a * b) % m);
}

/* F(e) mod m */
static uint64_t A214797_fib_mod(uint64_t e, uint64_t m)
{
    uint64_t a = 0, b = 1 % m;
    int i;
    for (i = 63; i >= 0; --i)
    {
        uint64_t t = (2 * (unsigned __int128)b + m - a) % m;
        uint64_t c = A214797_mm(a, t, m);
        uint64_t d = (A214797_mm(a, a, m) + A214797_mm(b, b, m)) % m;
        if ((e >> i) & 1)
        {
            a = d;
            b = (c + d) % m;
        }
        else
        {
            a = c;
            b = d;
        }
    }
    return a;
}

static int A214797_is_term(uint64_t n)
{
    uint64_t target = (n - (21 % n)) % n; /* F(m) must be == -21 mod n */
    uint64_t m;
    uint64_t fp, fc;
    if (A214797_fib_mod(n + 9, n) != target)
        return 0;
    /* ensure no smaller m in [1, n+8] */
    fp = 0;
    fc = 1 % n;
    for (m = 1; m <= n + 8; ++m)
    {
        if (fc == target)
            return 0;
        {
            uint64_t nx = fp + fc;
            if (nx >= n) nx -= n;
            fp = fc;
            fc = nx;
        }
    }
    return 1;
}

static uint64_t *A214797_terms = 0;
static size_t A214797_cnt = 0, A214797_cap = 0;
static uint64_t A214797_next = 1;

seq_t a(idx_t n)
{
    while (A214797_cnt < (size_t)n)
    {
        uint64_t c = A214797_next++;
        if (A214797_is_term(c))
        {
            if (A214797_cnt == A214797_cap)
            {
                A214797_cap = A214797_cap ? A214797_cap * 2 : 64;
                A214797_terms = (uint64_t *)realloc(A214797_terms, A214797_cap * sizeof(uint64_t));
            }
            A214797_terms[A214797_cnt++] = c;
        }
    }
    return (seq_t)A214797_terms[n - 1];
}
