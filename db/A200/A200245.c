/*
 * A200245  Partial sums of A200244.
 * https://oeis.org/A200245
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} [popcount(prime(k)) is even]
 * Range:     u8 n <= 628, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 86 terms through n = 978 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

static uint32_t *A200245_pr = NULL;   /* primes */
static uint32_t *A200245_cs = NULL;   /* cumulative counts */
static size_t A200245_sz = 0, A200245_cap = 0;
static uint32_t A200245_cand = 3;

static void A200245_extend(size_t k)
{
    while (A200245_sz < k) {
        uint32_t p;
        if (A200245_sz == 0) {
            p = 2;
        } else {
            for (;;) {
                bool ok = true;
                p = A200245_cand;
                for (size_t i = 0; i < A200245_sz; ++i) {
                    uint32_t q = A200245_pr[i];
                    if ((uint64_t)q * q > (uint64_t)p) break;
                    if (p % q == 0) { ok = false; break; }
                }
                A200245_cand += 2;
                if (ok) break;
            }
        }
        if (A200245_sz == A200245_cap) {
            A200245_cap = A200245_cap ? A200245_cap * 2 : 64;
            A200245_pr = (uint32_t *)realloc(A200245_pr, A200245_cap * sizeof(uint32_t));
            A200245_cs = (uint32_t *)realloc(A200245_cs, A200245_cap * sizeof(uint32_t));
            if (!A200245_pr || !A200245_cs) abort();
        }
        unsigned par = 0;
        uint32_t t = p;
        while (t) { par ^= 1; t &= t - 1; }
        uint32_t prev = A200245_sz ? A200245_cs[A200245_sz - 1] : 0;
        A200245_pr[A200245_sz] = p;
        A200245_cs[A200245_sz] = prev + (par ^ 1);
        A200245_sz++;
    }
}

seq_t a(idx_t n)
{
    A200245_extend((size_t)n);
    return (seq_t)A200245_cs[(size_t)n - 1];
}
