/*
 * A082271  Palindromes arising in A082270.
 * https://oeis.org/A082271
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = A082270(n)*A082270(n+1) + 1, where A082270(1) = 1 and
 *            A082270(k+1) is the smallest unused number with
 *            A082270(k)*A082270(k+1)+1 palindromic (computed by search)
 * Range:     u8 n <= 7, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Validated: partial, pass, 48 terms through n = 48 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static bool A082270_is_seen(const seq_t *s, size_t sz, seq_t val)
{
    for (size_t i = 0; i < sz; ++i)
        if (s[i] == val) return true;
    return false;
}

static bool A082270_is_pal(seq_t val)
{
    if (val == (seq_t)0) return true;
    static unsigned char digits[8192];
    size_t len = 0;
    seq_t tmp = val;
    while (tmp != (seq_t)0) {
        digits[len++] = (unsigned char)(tmp % (seq_t)10);
        tmp /= (seq_t)10;
    }
    for (size_t i = 0; i < len / 2; ++i)
        if (digits[i] != digits[len - 1 - i]) return false;
    return true;
}

static seq_t *A082270_cache = NULL;
static size_t A082270_size = 0;
static size_t A082270_cap = 0;

static void A082270_grow(void)
{
    size_t new_cap = (A082270_cap == 0) ? 16 : A082270_cap * 2;
    seq_t *ptr = mem_resize(seq_t, A082270_cache, new_cap);
    if (!ptr) { abort(); }
    A082270_cache = ptr;
    A082270_cap = new_cap;
}

static void A082270_ensure(idx_t n)
{
    if (n == 0) return;
    if (A082270_size >= (size_t)n) return;

    if (A082270_size == 0) {
        A082270_grow();
        A082270_cache[0] = (seq_t)1;
        A082270_size = 1;
    }

    while (A082270_size < (size_t)n) {
        seq_t prev = A082270_cache[A082270_size - 1];
        seq_t cand = (seq_t)1;
        while (1) {
            if (!A082270_is_seen(A082270_cache, A082270_size, cand)) {
                seq_t prod = prev * cand + (seq_t)1;
                if (A082270_is_pal(prod)) {
                    if (A082270_size == A082270_cap) A082270_grow();
                    A082270_cache[A082270_size++] = cand;
                    break;
                }
            }
            ++cand;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    A082270_ensure(n + 1);
    return A082270_cache[n - 1] * A082270_cache[n] + (seq_t)1;
}
