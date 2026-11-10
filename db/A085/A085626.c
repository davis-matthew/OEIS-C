/*
 * A085626  Partial sums of A051935.
 * https://oeis.org/A085626
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} A051935(k); computed by search: a(1) = 2,
 *            and each new term of A051935 is the smallest t > previous term
 *            such that a(n-1) + t is prime
 * Range:     u8 n <= 13, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Validated: partial, pass, 49 terms through n = 49 (2026-11-07)
 * Helpers:   is_prime_seq, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"
#include "../utils/is_prime_seq.h"

static seq_t *A085626_sums = NULL;
static idx_t A085626_len = 0;
static idx_t A085626_cap = 0;
static seq_t A085626_last = 0;

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    if (n > A085626_cap) {
        idx_t nc = A085626_cap ? A085626_cap : 64;
        while (nc < n) nc *= 2;
        A085626_sums = mem_resize(seq_t, A085626_sums, nc);
        A085626_cap = nc;
    }

    if (A085626_len == 0) {
        A085626_sums[0] = (seq_t)2;
        A085626_last = (seq_t)2;
        A085626_len = 1;
    }

    while (A085626_len < n) {
        seq_t sum = A085626_sums[A085626_len - 1];
        seq_t t = A085626_last + 1;
        while (!is_prime_seq(sum + t)) ++t;
        A085626_last = t;
        A085626_sums[A085626_len] = sum + t;
        ++A085626_len;
    }
    return A085626_sums[n - 1];
}
