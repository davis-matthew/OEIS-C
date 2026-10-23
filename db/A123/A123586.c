/*
 * A123586  Numbers that are not palindromes of 3 or more digits in some base
 *          b >= 2.
 * https://oeis.org/A123586
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) is the n‑th positive integer that is not a palindrome of
 *            length ≥3 in any base b ≥ 2
 * Range:     u8 n <= 24, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/floor_sqrt.h"

static seq_t *seq_array   = NULL;   /* stores computed terms */
static size_t seq_len     = 0;      /* number of terms stored */
static size_t seq_cap     = 0;      /* allocated capacity */
static seq_t next_candidate = 1;    /* next number to test */

/* return true if x has a palindrome representation of length ≥3 in some base 2≤b≤sqrt(x) */
static bool has_palindrome_in_any_base(seq_t x)
{
    if (x <= 4) return false;    /* 1,2,3,4 cannot have length ≥3 in any base */

    idx_t limit = (idx_t)floor_sqrt(x);   /* only need to try bases up to sqrt(x) */

    for (idx_t b = 2; b <= limit; ++b)
    {
        seq_t tmp = x;
        uint8_t digits[64];   /* enough for all bases of the numbers we test */
        size_t len = 0;

        while (tmp > 0)
        {
            seq_t base_seq = (seq_t)b;
            digits[len++] = (uint8_t)(tmp % base_seq);
            tmp /= base_seq;
        }

        if (len < 3) continue;     /* cannot be a palindrome of length ≥3 */

        bool is_pal = true;
        for (size_t i = 0; i * 2 + 1 < len; ++i)
            if (digits[i] != digits[len - 1 - i]) { is_pal = false; break; }

        if (is_pal) return true;   /* found a palindrome of required length */
    }
    return false;
}

/* ensure seq_array has capacity for new_len elements */
static void ensure_capacity(size_t new_len)
{
    if (new_len <= seq_cap) return;
    size_t new_cap = seq_cap == 0 ? 16 : seq_cap * 2;
    if (new_cap < new_len) new_cap = new_len;
    seq_array = (seq_t *)realloc(seq_array, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* generate terms until we have at least target_index terms (1‑based) */
static void generate_until(size_t target_index)
{
    while (seq_len < target_index)
    {
        if (!has_palindrome_in_any_base(next_candidate))
        {
            ensure_capacity(seq_len + 1);
            seq_array[seq_len++] = next_candidate;
        }
        next_candidate++;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* behaviour below offset does not matter */
    generate_until((size_t)n);
    return seq_array[n - 1];
}
