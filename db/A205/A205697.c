/*
 * A205697  Positions of multiples of 7 in A204890 (differences of primes).
 * https://oeis.org/A205697
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = position in A204890 (rows j=2,3,..., entries i=1..j-1,
 *            index (j-2)(j-1)/2 + i) of the n-th pair with prime(i) ==
 *            prime(j) (mod 7); found by direct search
 * Range:     u8 n <= 31, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Validated: partial, pass, 52 terms through n = 52 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

static uint32_t *A205697_pr = NULL;
static size_t A205697_sz = 0, A205697_cap = 0;

static void A205697_ensure(size_t k)
{
    if (A205697_cap == 0) {
        A205697_cap = 64;
        A205697_pr = (uint32_t *)malloc(A205697_cap * sizeof(uint32_t));
        if (!A205697_pr) abort();
        A205697_pr[0] = 2;
        A205697_sz = 1;
    }
    uint32_t cand = A205697_pr[A205697_sz - 1] + 1;
    if (cand < 3) cand = 3;
    if ((cand & 1u) == 0) cand++;
    while (A205697_sz < k) {
        bool ok = true;
        for (size_t i = 0; i < A205697_sz; ++i) {
            uint64_t q = A205697_pr[i];
            if (q * q > cand) break;
            if (cand % q == 0) { ok = false; break; }
        }
        if (ok) {
            if (A205697_sz == A205697_cap) {
                A205697_cap <<= 1;
                A205697_pr = (uint32_t *)realloc(A205697_pr, A205697_cap * sizeof(uint32_t));
                if (!A205697_pr) abort();
            }
            A205697_pr[A205697_sz++] = cand;
        }
        cand += 2;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    idx_t count = 0;
    for (idx_t j = 2;; ++j) {
        A205697_ensure((size_t)j);
        uint32_t rj = A205697_pr[j - 1] % 7u;
        for (idx_t i = 1; i < j; ++i) {
            if (A205697_pr[i - 1] % 7u == rj) {
                if (++count == n) {
                    idx_t pos = (j - 2) * (j - 1) / 2 + i;
                    return (seq_t)pos;
                }
            }
        }
    }
}
