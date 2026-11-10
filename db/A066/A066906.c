/*
 * A066906  Places n where A006577(n) is a prime number.
 * https://oeis.org/A066906
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k >= 1 such that A006577(k) is prime; found by
 *            incremental search with cached results
 * Range:     u8 n <= 22, u16 n <= 626, u32 n <= 2201, u64 n <= 2201,
 *            u128 n <= 2201, big any n
 * Validated: standard, pass, 76 terms through n = 2175 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static unsigned long A066906_steps(idx_t k)
{
    seq_t x = (seq_t)k;
    unsigned long steps = 0;
    while (x != 1) {
        if (x % 2 == 0) {
            x = x / 2;
        } else {
            x = 3 * x + 1;
        }
        steps++;
    }
    return steps;
}

static bool A066906_small_prime(unsigned long s)
{
    unsigned long d;
    if (s < 2) return false;
    for (d = 2; d * d <= s; d++) {
        if (s % d == 0) return false;
    }
    return true;
}

static idx_t *A066906_terms = NULL;
static size_t A066906_count = 0;
static size_t A066906_cap = 0;
static idx_t A066906_next = 1;

seq_t a(idx_t n)
{
    while (A066906_count < (size_t)n) {
        idx_t k = A066906_next++;
        if (A066906_small_prime(A066906_steps(k))) {
            if (A066906_count == A066906_cap) {
                size_t nc = A066906_cap ? A066906_cap * 2 : 1024;
                A066906_terms = (idx_t *)realloc(A066906_terms, nc * sizeof(idx_t));
                A066906_cap = nc;
            }
            A066906_terms[A066906_count++] = k;
        }
    }
    return (seq_t)A066906_terms[n - 1];
}
