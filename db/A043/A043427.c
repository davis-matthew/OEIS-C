/*
 * A043427  Numbers having three 1's in base 8.
 * https://oeis.org/A043427
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Validated: partial, pass, 42 terms through n = 42 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers having three 1's in base 8
 * The sequence is defined as the increasing list of integers whose
 * base‑8 representation contains exactly three digits equal to 1.
 * Example: 73₁₀ = 111₈, 521₁₀ = 1011₈, 586₁₀ = 1112₈, …
 */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

static inline int count_ones_in_octal(uint64_t x)
{
    int cnt = 0;
    while (x && cnt <= 3) {
        if ((x % 8) == 1) {
            ++cnt;
        }
        x /= 8;
    }
    return cnt;
}

static seq_t *seq_cache = NULL;
static size_t seq_cap   = 0;
static size_t seq_len   = 0;
static uint64_t next_num = 1;

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset 1 – undefined for n<1 */

    /* Grow cache until it contains at least n elements */
    while (seq_len < (size_t)n) {
        /* Ensure the array has enough space */
        if (seq_len == seq_cap) {
            size_t new_cap = seq_cap ? seq_cap * 2 : 16;
            seq_t *tmp = mem_resize(seq_t, seq_cache, new_cap);
            if (!tmp) { /* allocation failed, return 0 as a safe error value */
                return (seq_t)0;
            }
            seq_cache = tmp;
            seq_cap   = new_cap;
        }

        /* Check the current number */
        if (count_ones_in_octal(next_num) == 3) {
            seq_cache[seq_len++] = (seq_t)next_num;
        }
        ++next_num;
    }

    return seq_cache[n - 1];
}
