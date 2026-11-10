/*
 * A229291  n is in the sequence if n is prime, (n-1)/5^A112765(n-1) is a
 *          squarefree number, A112765(n-1) < 3 and every prime divisor of n-1
 *          is in the sequence.
 * https://oeis.org/A229291
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search over primes p = 2, 3, 5, ...; p is a term if it is 2, 3
 *            or 5, or if (p-1)/5^A112765(p-1) is squarefree, A112765(p-1) <
 *            3, and every prime divisor of p-1 is itself a term (checked
 *            recursively)
 * Range:     u8 n <= 14, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Validated: partial, pass, 50 terms through n = 50 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static bool A229291_isprime(uint64_t m)
{
    if (m < 2) return false;
    if (m % 2 == 0) return m == 2;
    if (m % 3 == 0) return m == 3;
    for (uint64_t d = 5; d * d <= m; d += 6)
    {
        if (m % d == 0 || m % (d + 2) == 0) return false;
    }
    return true;
}

static bool A229291_member(uint64_t m)
{
    if (m == 2 || m == 3 || m == 5) return true;
    if (!A229291_isprime(m)) return false;
    uint64_t x = m - 1;
    uint64_t v = 0;
    while (x % 5 == 0) { v++; x /= 5; }
    if (v >= 3) return false;
    for (uint64_t q = 2; q * q <= x; q++)
    {
        if (x % q == 0)
        {
            x /= q;
            if (x % q == 0) return false;
            if (!A229291_member(q)) return false;
        }
    }
    if (x > 1 && !A229291_member(x)) return false;
    return true;
}

static uint64_t *A229291_terms = 0;
static size_t A229291_cnt = 0, A229291_cap = 0;
static uint64_t A229291_next = 2;

seq_t a(idx_t n)
{
    while (A229291_cnt < (size_t)n)
    {
        uint64_t m = A229291_next;
        A229291_next = (m == 2) ? 3 : m + 2;
        if (A229291_member(m))
        {
            if (A229291_cnt == A229291_cap)
            {
                A229291_cap = A229291_cap ? A229291_cap * 2 : 256;
                A229291_terms = (uint64_t *)realloc(A229291_terms, A229291_cap * sizeof(uint64_t));
            }
            A229291_terms[A229291_cnt++] = m;
        }
    }
    return (seq_t)A229291_terms[n - 1];
}
