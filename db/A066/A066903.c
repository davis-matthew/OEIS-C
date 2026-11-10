/*
 * A066903  Primes in A006577.
 * https://oeis.org/A066903
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = n-th prime among the values of A006577(m), m = 1, 2, 3,
 *            ... (search with memoized scan)
 * Range:     u8 n <= 2762, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 83 terms through n = 8889 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint32_t *A066903_terms = NULL;
static size_t A066903_cnt = 0, A066903_cap = 0;
static uint64_t A066903_next_m = 1;

static uint32_t A066903_collatz(uint64_t m)
{
    unsigned __int128 x = m;
    uint32_t steps = 0;
    while (x != 1) {
        if ((x & 1) == 0) x >>= 1;
        else x = 3 * x + 1;
        steps++;
    }
    return steps;
}

static bool A066903_isprime(uint32_t v)
{
    if (v < 2) return false;
    if (v < 4) return true;
    if (v % 2 == 0) return false;
    for (uint32_t d = 3; (uint64_t)d * d <= v; d += 2)
        if (v % d == 0) return false;
    return true;
}

seq_t a(idx_t n)
{
    size_t need = (size_t)n;
    while (A066903_cnt < need) {
        uint32_t s = A066903_collatz(A066903_next_m++);
        if (A066903_isprime(s)) {
            if (A066903_cnt == A066903_cap) {
                size_t nc = A066903_cap ? A066903_cap * 2 : 1024;
                A066903_terms = (uint32_t *)realloc(A066903_terms, nc * sizeof(uint32_t));
                A066903_cap = nc;
            }
            A066903_terms[A066903_cnt++] = s;
        }
    }
    return (seq_t)A066903_terms[need - 1];
}
