/*
 * A077358  Duplicate of A069686.
 * https://oeis.org/A077358
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = n-th prime p with at least 3 digits such that p with its
 *            first and last digit removed is prime (search; duplicate of
 *            A069686)
 * Range:     i8 n <= 1, i16 n <= 17, i32 n <= 17, i64 n <= 17, i128 n <= 17,
 *            big any n
 * Validated: partial, pass, 17 terms through n = 17 (2026-11-07)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

static seq_t *A077358_cache = NULL;
static size_t A077358_len = 0;
static size_t A077358_cap = 0;
static uint64_t A077358_next = 100;

static bool A077358_ok(uint64_t p)
{
    if (!is_prime_u64(p)) return false;
    uint64_t q = p / 10;          /* drop last digit */
    uint64_t pw = 1;
    uint64_t t = q;
    while (t >= 10) { t /= 10; pw *= 10; }   /* pw = 10^(digits(q)-1) */
    uint64_t inner = q % pw;      /* drop first digit */
    if (inner < 2) return false;
    return is_prime_u64(inner);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    while ((idx_t)A077358_len < n) {
        uint64_t p = A077358_next++;
        if (!A077358_ok(p)) continue;
        if (A077358_len == A077358_cap) {
            size_t nc = A077358_cap ? A077358_cap * 2 : 64;
            seq_t *tmp = mem_resize(seq_t, A077358_cache, nc);
            if (!tmp) return 0;
            A077358_cache = tmp;
            A077358_cap = nc;
        }
        A077358_cache[A077358_len++] = (seq_t)p;
    }
    return A077358_cache[n - 1];
}
