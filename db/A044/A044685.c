/*
 * A044685  Numbers n such that string 5,8 occurs in the base 9 representation
 *          of n but not of n+1.
 * https://oeis.org/A044685
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth integer n ≥ 1 such that the base‑9 representation
 *            of n contains the consecutive digits “5,8” but the
 *            representation of n+1 does not
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
#include <vector>
#endif

/* Utility: test whether a given value contains the substring
   5 followed immediately by 8 in its base-9 representation.
   For a base‑9 string left‑to‑right 5…8, the least significant
   digit comes first when we extract digits by modulo 9, so
   we look for ...858 in the extracted array. */
static bool has58_base9(seq_t v)
{
    /* Maximum digits for a 128‑bit integer in base 9 is < 64. */
    uint8_t digits[64];
    size_t len = 0;
    while (v > (seq_t)0) {
        digits[len++] = (uint8_t)(v % (seq_t)9);
        v /= (seq_t)9;
    }
    if (len < 2) return false;
    for (size_t i = 0; i + 1 < len; ++i)
        if (digits[i] == 8 && digits[i + 1] == 5)
            return true;
    return false;
}

#ifdef __cplusplus
/* C++ implementation: use std::vector for proper construction. */
static std::vector<seq_t> seq;      /* collected valid numbers */
static idx_t next_start = 1;        /* next integer to test */
static idx_t cache_cap = 0;         /* capacity in terms of integers tested */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for index 0, but return 0 */
    while ((idx_t)seq.size() < n) {
        seq_t val = (seq_t)next_start;
        if (has58_base9(val) && !has58_base9(val + (seq_t)1))
            seq.push_back(val);
        ++next_start;
    }
    return seq[n - 1];
}
#else
/* C implementation: dynamic array with realloc. */
static seq_t *seq = NULL;          /* collected valid numbers */
static idx_t seq_len = 0;          /* number of collected terms */
static idx_t seq_cap = 0;          /* array capacity */
static idx_t next_start = 1;       /* next integer to test */

static void seq_realloc_if_needed(void)
{
    if (seq_len == seq_cap) {
        idx_t new_cap = seq_cap ? seq_cap * 2 : 16;
        seq_t *tmp = realloc(seq, new_cap * sizeof(seq_t));
        seq = tmp;
        seq_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for index 0 */
    while (seq_len < n) {
        seq_t val = (seq_t)next_start;
        if (has58_base9(val) && !has58_base9(val + (seq_t)1)) {
            seq_realloc_if_needed();
            seq[seq_len++] = val;
        }
        ++next_start;
    }
    return seq[n - 1];
}
#endif
