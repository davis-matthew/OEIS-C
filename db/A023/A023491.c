/*
 * A023491  a(n) = b(n) + d(n), where b(n) = (n-th Fibonacci number > 1) and
 *          d(n) = (n-th non-Lucas number).
 * https://oeis.org/A023491
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Fib(n+3) + d(n) where d(n) is the n-th non‑Lucas number
 *            (Lucas numbers with L(0)=1, L(1)=3) starting from 2
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/fib.h"

/* ------------------------------------------------------------------ */
/* Helper: generate Lucas numbers (variant L0=1, L1=3) up to a limit. */
static uint64_t *lucas_nums = NULL;
static size_t lucas_len  = 0;
static size_t lucas_cap  = 0;

static void _ensure_lucas_upto(uint64_t limit)
{
    if (lucas_len == 0) {
        lucas_cap = 8;
        lucas_nums = (uint64_t *)malloc(lucas_cap * sizeof(uint64_t));
        if (!lucas_nums) abort();
        lucas_nums[0] = 1;
        lucas_nums[1] = 3;
        lucas_len   = 2;
    }
    /* Generate while last Lucas number is less than required limit. */
    while (lucas_len < lucas_cap && lucas_nums[lucas_len - 1] < limit) {
        if (lucas_len == lucas_cap) {
            lucas_cap <<= 1;
            lucas_nums = (uint64_t *)realloc(lucas_nums,
                                            lucas_cap * sizeof(uint64_t));
            if (!lucas_nums) abort();
        }
        uint64_t next = lucas_nums[lucas_len - 1] + lucas_nums[lucas_len - 2];
        lucas_nums[lucas_len++] = next;
    }
    /* If last element still < limit, keep generating. */
    while (lucas_len < lucas_cap && lucas_nums[lucas_len - 1] < limit) {
        if (lucas_len == lucas_cap) {
            lucas_cap <<= 1;
            lucas_nums = (uint64_t *)realloc(lucas_nums,
                                            lucas_cap * sizeof(uint64_t));
            if (!lucas_nums) abort();
        }
        uint64_t next = lucas_nums[lucas_len - 1] + lucas_nums[lucas_len - 2];
        lucas_nums[lucas_len++] = next;
    }
    /* If we still don't have enough (rare), continue generating. */
    while (lucas_nums[lucas_len - 1] < limit) {
        if (lucas_len == lucas_cap) {
            lucas_cap <<= 1;
            lucas_nums = (uint64_t *)realloc(lucas_nums,
                                            lucas_cap * sizeof(uint64_t));
            if (!lucas_nums) abort();
        }
        uint64_t next = lucas_nums[lucas_len - 1] + lucas_nums[lucas_len - 2];
        lucas_nums[lucas_len++] = next;
    }
}

static bool _is_lucas_u64(uint64_t x)
{
    if (x < 1) return false;          /* 0 is not a Lucas number here. */
    _ensure_lucas_upto(x);
    /* Linear scan – lucas_len stays tiny (≤ 60 for values <10^20). */
    for (size_t i = 0; i < lucas_len; ++i)
        if (lucas_nums[i] == x)
            return true;
    return false;
}

/* ------------------------------------------------------------------ */
/* Compute the n-th non‑Lucas number (offset 0, first term = 2). */
static seq_t _nonlucas_from_index(idx_t n)
{
    uint64_t count = 0;
    uint64_t m = 2;                    /* start scanning from 2 */
    while (true) {
        if (!_is_lucas_u64(m)) {       /* skip Lucas numbers */
            if (count == n)
                break;
            ++count;
        }
        ++m;
    }
    return (seq_t)m;
}

/* ------------------------------------------------------------------ */
/* Main sequence function. */
seq_t a(idx_t n)
{
    /* b(n) = Fibonacci(n+3) (first index 0 gives F(3)=2). */
    seq_t b = fib(n + (idx_t)3);
    seq_t d = _nonlucas_from_index(n);
    return b + d;
}
