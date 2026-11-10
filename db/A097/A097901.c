/*
 * A097901  Duplicate of A026427.
 * https://oeis.org/A097901
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(1)=3; a(n) = least integer > a(n-1) not equal to a(i)*a(j) for
 *            1<=i<=j<n (duplicate of A026427; computed by search with a
 *            bitset of taken values)
 * Range:     i8 n <= 25, i16 n <= 25, i32 n <= 25, i64 n <= 25, i128 n <= 25,
 *            big any n
 * Validated: partial, pass, 25 terms through n = 25 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/mem.h"

static seq_t *A026427_seq = NULL;
static size_t A026427_len = 0;
static size_t A026427_cap = 0;

static unsigned char *A026427_present = NULL;
static size_t A026427_present_bits = 0;

static void A026427_ensure_present(seq_t value)
{
    size_t v = (size_t)value;
    if (v < A026427_present_bits) return;
    size_t new_bits = v + 1;
    size_t old_bytes = (A026427_present_bits + 7) >> 3;
    size_t new_bytes = (new_bits + 7) >> 3;
    A026427_present = (unsigned char *)realloc(A026427_present, new_bytes);
    if (!A026427_present) exit(1);
    if (new_bytes > old_bytes)
        memset(A026427_present + old_bytes, 0, new_bytes - old_bytes);
    A026427_present_bits = new_bits;
}

static void A026427_set_present(seq_t value)
{
    A026427_ensure_present(value);
    size_t v = (size_t)value;
    A026427_present[v >> 3] |= (unsigned char)(1 << (v & 7));
}

static bool A026427_is_present(seq_t value)
{
    size_t v = (size_t)value;
    if (v >= A026427_present_bits) return false;
    return (A026427_present[v >> 3] & (1 << (v & 7))) != 0;
}

static void A026427_append(seq_t val)
{
    if (A026427_len + 1 > A026427_cap) {
        size_t new_cap = A026427_cap ? A026427_cap * 2 : 4;
        A026427_seq = mem_resize(seq_t, A026427_seq, new_cap);
        if (!A026427_seq) exit(1);
        A026427_cap = new_cap;
    }
    A026427_seq[A026427_len++] = val;
    A026427_set_present(val);
}

static seq_t A026427_a(idx_t n)
{
    if (n == 0) return 0;
    size_t target = (size_t)n;
    if (A026427_len >= target) return A026427_seq[target - 1];

    while (A026427_len < target) {
        seq_t candidate;
        if (A026427_len == 0)
            candidate = (seq_t)3;
        else
            candidate = A026427_seq[A026427_len - 1] + (seq_t)1;

        while (true) {
            bool allowed = true;
            for (size_t i = 0; i < A026427_len; ++i) {
                seq_t ai = A026427_seq[i];
                if (ai > candidate) break;
                if (candidate % ai == (seq_t)0) {
                    seq_t y = candidate / ai;
                    if (y < ai) continue;
                    if (A026427_is_present(y)) {
                        allowed = false;
                        break;
                    }
                }
            }
            if (allowed) break;
            ++candidate;
        }
        A026427_append(candidate);
    }
    return A026427_seq[target - 1];
}

seq_t a(idx_t n)
{
    return A026427_a(n);
}
