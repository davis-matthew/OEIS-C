/*
 * A202393  Digital root of Hoax Numbers A019506.
 * https://oeis.org/A202393
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A010888(A019506(n)); computed by sieve/search for hoax
 *            numbers, then digital root = 1 + (m-1) mod 9
 * Range:     u8 n <= 11, u16 n <= 3407, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 102 terms through n = 9393 (2026-11-07)
 * Helpers:   digit_sum, is_prime_seq, mem, smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include <stddef.h>
#include "../utils/digit_sum.h"
#include "../utils/is_prime_seq.h"
#include "../utils/smallest_prime_factor.h"
#include "../utils/mem.h"

static seq_t *A019506_cache = NULL;
static size_t A019506_len = 0;
static size_t A019506_cap = 0;
static uint64_t A019506_last = 3;

static unsigned int A019506_pds(uint64_t x)
{
    unsigned int sum = 0;
    uint64_t m = x;
    uint64_t last_p = 0;
    while (m > 1) {
        uint64_t p = smallest_prime_factor(m);
        if (p != last_p) {
            sum += (unsigned int)digit_sum((seq_t)p);
            last_p = p;
        }
        do {
            m /= p;
        } while (m % p == 0);
    }
    return sum;
}

static seq_t A019506_a(idx_t n)
{
    if (A019506_cap == 0) {
        A019506_cap = 64;
        A019506_cache = mem_new(seq_t, A019506_cap);
        assert(A019506_cache != NULL);
    }
    while (A019506_len < (size_t)n) {
        uint64_t x = ++A019506_last;
        if (is_prime_seq((seq_t)x))
            continue;
        unsigned int cv = (unsigned int)digit_sum((seq_t)x);
        if (A019506_pds(x) == cv) {
            if (A019506_len == A019506_cap) {
                A019506_cap *= 2;
                seq_t *np = mem_resize(seq_t, A019506_cache, A019506_cap);
                assert(np != NULL);
                A019506_cache = np;
            }
            A019506_cache[A019506_len++] = (seq_t)x;
        }
    }
    return A019506_cache[n - 1];
}

seq_t a(idx_t n)
{
    seq_t v = A019506_a(n);
    seq_t r = (seq_t)(v % 9);
    return (r == 0) ? (seq_t)9 : r;
}
