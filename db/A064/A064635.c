/*
 * A064635  Even numbers > 6 not appearing in A064466. a(n) =
 *          A064466(A064634(n)) + 2 for n > 0.
 * https://oeis.org/A064635
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th even number > 6 not appearing in A064466, where
 *            A064466 is b(0)=6, b(k) = min{ m > b(k-1) : b(k-1)-p and m-p
 *            both prime for some prime p }; computed by generating A064466
 *            and collecting the even numbers it skips
 * Range:     u8 n <= 15, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Validated: partial, pass, 52 terms through n = 52 (2026-11-07)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/is_prime_u64.h"

static uint64_t A064635_next_prime(uint64_t n)
{
    uint64_t c = n + 1;
    if (c <= 2) return 2;
    if ((c & 1) == 0) c++;
    while (!is_prime_u64(c)) c += 2;
    return c;
}

static uint64_t A064635_step(uint64_t prev)
{
    uint64_t best = 0;
    if (prev >= 4) {
        for (uint64_t p = 2; p + 2 <= prev; p = A064635_next_prime(p)) {
            uint64_t q = prev - p;
            if (!is_prime_u64(q)) continue;
            uint64_t m = p + A064635_next_prime(q);
            if (best == 0 || m < best) best = m;
        }
    }
    return best ? best : prev + 2;
}

static uint64_t *A064635_miss = NULL;
static size_t A064635_len = 0, A064635_cap = 0;
static uint64_t A064635_cur = 6;

seq_t a(idx_t n)
{
    size_t need = (size_t)n;
    while (A064635_len < need) {
        uint64_t nxt = A064635_step(A064635_cur);
        for (uint64_t e = A064635_cur + 1; e < nxt; e++) {
            if ((e & 1) == 0 && e > 6) {
                if (A064635_len == A064635_cap) {
                    size_t nc = A064635_cap ? A064635_cap * 2 : 64;
                    A064635_miss = (uint64_t *)realloc(A064635_miss, nc * sizeof(uint64_t));
                    A064635_cap = nc;
                }
                A064635_miss[A064635_len++] = e;
            }
        }
        A064635_cur = nxt;
    }
    return (seq_t)A064635_miss[need - 1];
}
