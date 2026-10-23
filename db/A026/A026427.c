/*
 * A026427  a(n) = least positive integer > a(n-1) and not equal to a(i)*a(j)
 *          for 1<=i<=j<=n.
 * https://oeis.org/A026427
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 123, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition: a(1)=3; for n>1, a(n) is the least integer > a(n-1)
 * that is not equal to a(i)*a(j) for any 1<=i<=j<=n-1. */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

static seq_t *a_seq = NULL;          /* stored a(1)... */
static size_t  a_len  = 0;           /* current number of stored terms */
static size_t  a_cap  = 0;           /* allocated capacity for a_seq */

static unsigned char *present = NULL;/* bitset of values already taken   */
static size_t present_bits = 0;      /* number of bits allocated          */

/* Ensure present has enough bits to index value */
static void ensure_present_capacity(seq_t value)
{
    if (value < present_bits) return;
    size_t new_bits = (size_t)value + 1;          /* +1 because value itself must be indexed */
    size_t old_bytes = (present_bits + 7) >> 3;
    size_t new_bytes = (new_bits + 7) >> 3;
    present = (unsigned char*)realloc(present, new_bytes);
    if (!present) exit(1);      /* out of memory */
    if (new_bytes > old_bytes)
        memset(present + old_bytes, 0, new_bytes - old_bytes);
    present_bits = new_bits;
}

/* Set bit for value */
static void set_present(seq_t value)
{
    ensure_present_capacity(value);
    size_t byte_idx = (size_t)value >> 3;
    unsigned char mask   = 1 << ((size_t)value & 7);
    present[byte_idx] |= mask;
}

/* Query if value has been taken */
static bool is_present(seq_t value)
{
    if ((size_t)value >= present_bits) return false;
    size_t byte_idx = (size_t)value >> 3;
    unsigned char mask = 1 << ((size_t)value & 7);
    return (present[byte_idx] & mask) != 0;
}

/* Ensure enough capacity in a_seq */
static void ensure_seq_capacity(size_t needed)
{
    if (needed <= a_cap) return;
    size_t new_cap = a_cap ? a_cap * 2 : 4;
    if (new_cap < needed) new_cap = needed;
    a_seq = (seq_t*)realloc(a_seq, new_cap * sizeof(seq_t));
    if (!a_seq) exit(1);
    a_cap = new_cap;
}

/* Append new value */
static void append_value(seq_t val)
{
    ensure_seq_capacity(a_len + 1);
    a_seq[a_len++] = val;
    set_present(val);
}

seq_t a(idx_t n)
{
    if (n == 0) { /* offset is 1; n>=1 always in tests */
        return 0;
    }
    size_t target = (size_t)n;
    if (a_len >= target) return a_seq[target - 1];

    /* Build terms up to n */
    while (a_len < target) {
        /* Determine next candidate */
        seq_t candidate;
        if (a_len == 0)
            candidate = (seq_t)3;
        else
            candidate = a_seq[a_len - 1] + (seq_t)1;

        /* Find first integer > last that is not forbidden */
        while (true) {
            bool allowed = true;
            for (size_t i = 0; i < a_len; ++i) {
                seq_t ai = a_seq[i];
                if (ai > candidate) break;
                if (candidate % ai == (seq_t)0) {
                    seq_t y = candidate / ai;
                    if (y < ai) continue;
                    if (is_present(y)) {
                        allowed = false;
                        break;
                    }
                }
            }
            if (allowed) break;                 /* candidate is accepted */
            ++candidate;
        }
        append_value(candidate);                 /* store new term */
    }
    return a_seq[n - 1];
}
