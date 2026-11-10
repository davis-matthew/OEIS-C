/*
 * A050665  Primes p such that number of primes produced according to rules
 *          stipulated in Honaker's A048853 is 3.
 * https://oeis.org/A050665
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th prime p with A048853(p) = 3, where A048853(p) is the
 *            number of primes obtained by changing one decimal digit of p;
 *            found by a search
 * Range:     u8 n <= 4, u16 n <= 38, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Validated: partial, pass, 38 terms through n = 38 (2026-11-07)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_u64.h"

static uint64_t *A050665_terms = NULL;
static size_t A050665_cnt = 0, A050665_cap = 0;
static uint64_t A050665_next = 2;

static int A050665_count(uint64_t p)
{
    int count = 0;
    uint64_t pw = 1;
    uint64_t t = p;
    while (t > 0) {
        int dig = (int)(t % 10);
        t /= 10;
        for (int nd = 0; nd <= 9; ++nd) {
            if (nd == dig) continue;
            if (nd == 0 && t == 0) continue; /* leading zero */
            uint64_t cand = p - (uint64_t)dig * pw + (uint64_t)nd * pw;
            if (is_prime_u64(cand)) {
                ++count;
                if (count > 3) return count;
            }
        }
        pw *= 10;
    }
    return count;
}

seq_t a(idx_t n)
{
    while (A050665_cnt < (size_t)n) {
        uint64_t p = A050665_next++;
        if (!is_prime_u64(p)) continue;
        if (A050665_count(p) != 3) continue;
        if (A050665_cnt == A050665_cap) {
            A050665_cap = A050665_cap ? A050665_cap * 2 : 64;
            A050665_terms = (uint64_t *)realloc(A050665_terms, A050665_cap * sizeof(uint64_t));
        }
        A050665_terms[A050665_cnt++] = p;
    }
    return (seq_t)A050665_terms[n - 1];
}
