/*
 * A099677  Primes arising in A032682.
 * https://oeis.org/A099677
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 1k1 (digit 1, then k, then digit 1) where k = A032682(n)
 *            is the n-th k with 1k1 prime, except a(1) = 11 (k = 0
 *            contributes the empty-k term 11); computed by search
 * Range:     u8 n <= 5, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Validated: partial, pass, 43 terms through n = 43 (2026-11-07)
 * Helpers:   is_prime_seq, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/mem.h"
#include "../utils/is_prime_seq.h"

static seq_t *cache99677 = NULL;
static size_t cache99677_cap = 0;
static size_t cache99677_len = 0;
static idx_t k99677_next = 0;

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    while (cache99677_len < n) {
        idx_t k = k99677_next++;
        seq_t pw = 10;
        while ((seq_t)k >= pw) pw *= 10;
        seq_t N = pw * 10 + (seq_t)k * 10 + 1;
        if (is_prime_seq(N)) {
            if (cache99677_len + 1 > cache99677_cap) {
                size_t nc = cache99677_cap ? cache99677_cap * 2 : 64;
                seq_t *t = mem_resize(seq_t, cache99677, nc);
                if (!t) exit(EXIT_FAILURE);
                cache99677 = t;
                cache99677_cap = nc;
            }
            cache99677[cache99677_len++] = (k == 0) ? (seq_t)11 : N;
        }
    }
    return cache99677[n - 1];
}
