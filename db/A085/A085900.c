/*
 * A085900  Numbers k such that k-th cyclotomic polynomial has exactly 3
 *          negative coefficients.
 * https://oeis.org/A085900
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 96, u32 n <= 96, u64 n <= 96, u128 n <= 96,
 *            big any n
 * Validated: standard, pass, 68 terms through n = 96 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A085900:  numbers k such that the k‑th cyclotomic polynomial Φₖ(x)
   has exactly three negative coefficients.
   Characterisation (proved on OEIS):
     • k = 2^a · 7^b   with a≥1 and b≥1
       (all other prime factors are absent);
     • OR the odd part of k is 3^c · 5^d   with c≥1 and d≥1
       (the power of 2 may be zero or positive, no other primes). */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Test whether a number satisfies the above characterisation. */
static bool has_three_negative_coeffs(uint64_t k)
{
    if (k < 14)          /* the smallest such number is 14 */
        return false;

    uint64_t x = k;
    int e2 = 0, e3 = 0, e5 = 0, e7 = 0;

    while ((x & 1U) == 0) {
        ++e2;
        x >>= 1;
    }
    while (x % 3U == 0) {
        ++e3;
        x /= 3U;
    }
    while (x % 5U == 0) {
        ++e5;
        x /= 5U;
    }
    while (x % 7U == 0) {
        ++e7;
        x /= 7U;
    }

    if (x != 1U)                /* other prime factor present */
        return false;

    if (e7 > 0)                 /* case: 2^a · 7^b with a≥1,b≥1 */
        return (e2 >= 1) && (e3 == 0) && (e5 == 0);

    if (e3 > 0 && e5 > 0)       /* case: odd part 3^c · 5^d, c≥1,d≥1 */
        return true;

    return false;
}

seq_t a(idx_t n)
{
    /* static storage for the generated subsequence */
    static seq_t *seq = NULL;
    static size_t len = 0;
    static size_t cap = 0;
    static uint64_t cur = 2U;   /* next number to test */

    /* grow the array as needed */
    if (seq == NULL) {
        cap = 16;
        seq = mem_new(seq_t, cap);
    }

    while (len <= n) {
        while (!has_three_negative_coeffs(cur))
            ++cur;
        if (len == cap) {
            cap <<= 1;          /* double capacity */
            seq = mem_resize(seq_t, seq, cap);
        }
        seq[len++] = (seq_t)cur;
        ++cur;
    }

    return seq[n];
}
