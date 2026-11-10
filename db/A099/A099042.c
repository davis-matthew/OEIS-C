/*
 * A099042  Number of badly sieved (A066680) divisors of n.
 * https://oeis.org/A099042
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} A099104(d), where A099104(1)=0 and A099104(d) =
 *            Prod(if d>e^2 then 1 else 1-A099104(e) : e<d, e|d)
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9754 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

static unsigned char *A099042_st = NULL;   /* 0 unknown, 1 false, 2 true */
static size_t A099042_sz = 0;

static void A099042_ensure(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (needed <= A099042_sz)
        return;
    size_t ns = A099042_sz ? A099042_sz : 16;
    while (ns < needed)
        ns <<= 1;
    unsigned char *p = (unsigned char *)realloc(A099042_st, ns);
    if (!p)
        exit(1);
    for (size_t i = A099042_sz; i < ns; ++i)
        p[i] = 0;
    A099042_st = p;
    A099042_sz = ns;
}

/* e*e >= d without overflow */
static bool A099042_sqge(idx_t e, idx_t d)
{
    idx_t q = d / e;
    return e > q || (e == q && d % e == 0) || (e > q);
}

static bool A099042_bs(idx_t d)
{
    if (d == 1)
        return false;
    A099042_ensure(d);
    if (A099042_st[d])
        return A099042_st[d] == 2;

    bool result = true;
    for (idx_t i = 1; i <= d / i && result; ++i) {
        if (d % i != 0)
            continue;
        idx_t e1 = i;
        if (e1 < d && A099042_sqge(e1, d) && A099042_bs(e1)) {
            result = false;
            break;
        }
        idx_t e2 = d / i;
        if (e2 != e1 && e2 < d && A099042_sqge(e2, d) && A099042_bs(e2)) {
            result = false;
            break;
        }
    }
    A099042_st[d] = result ? 2 : 1;
    return result;
}

seq_t a(idx_t n)
{
    seq_t count = 0;
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i != 0)
            continue;
        idx_t d2 = n / i;
        if (A099042_bs(i))
            count++;
        if (d2 != i && A099042_bs(d2))
            count++;
    }
    return count;
}
