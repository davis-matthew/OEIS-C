/*
 * A030475  Primes with property that all even digits occur together and all
 *          odd digits occur together.
 * https://oeis.org/A030475
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth prime whose decimal digits contain at most one change
 *            of parity (the digits are grouped into a single block of evens
 *            followed by a single block of odds or vice‑versa). The function
 *            lazily generates the sequence by testing successive integers for
 *            primality and the parity‑block property,...
 * Range:     u8 n <= 45, u16 n <= 2222, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 72 terms through n = 9924 (2026-11-03)
 * Helpers:   is_prime_seq, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime_seq.h"
#include "../utils/mem.h"

static bool has_parity_block_property(uint64_t n)
{
    int last_parity = -1;   /* -1: undefined, 0: even, 1: odd */
    int changes = 0;
    while (n > 0) {
        int d = n % 10;
        int parity = d & 1;              /* 0 even, 1 odd */
        if (last_parity == -1) {
            last_parity = parity;
        } else if (parity != last_parity) {
            ++changes;
            if (changes > 1)
                return false;
            last_parity = parity;
        }
        n /= 10;
    }
    return true;
}

static seq_t *cache = NULL;        /* memoised primes */
static size_t cache_size = 0;      /* number of stored primes */
static size_t cache_cap = 0;       /* allocated capacity */
static uint64_t next_candidate = 2;/* next integer to test */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    while (cache_size < (size_t)n) {
        while (true) {
            if (is_prime_seq((seq_t)next_candidate) &&
                has_parity_block_property(next_candidate)) {
                if (cache_size == cache_cap) {
                    cache_cap = cache_cap ? cache_cap * 2 : 4;
                    seq_t *tmp = mem_resize(seq_t, cache, cache_cap);
                    if (!tmp) {
                        /* Allocation failed; return zero as a safe fallback. */
                        return 0;
                    }
                    cache = tmp;
                }
                cache[cache_size++] = (seq_t)next_candidate;
                ++next_candidate; /* advance for future searches */
                break;
            }
            ++next_candidate;
        }
    }

    return cache[n - 1];
}
