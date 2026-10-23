/*
 * A145101  Integers in which no digit occurs more than once more often than
 *          any other digit and not all repeated digits are identical, for all
 *          bases up to ten.
 * https://oeis.org/A145101
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A145101: 
 *   Integers which, in every base from 2 to 10, 
 *   (1) the frequencies of the digits that actually appear differ by at most one, 
 *   and 
 *   (2) no base representation consists of a single repeated digit (e.g. "11", "222").
 *   The sequence starts 1, 2, 17, 19, 25, 38, 52, ...
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static seq_t *seq = NULL;             /* array with seq[0] = placeholder */
static size_t seq_cap = 0;           /* current allocated capacity in elements */
static size_t seq_len = 1;           /* number of elements stored, including placeholder */
static seq_t next_candidate = 1;     /* next integer to test */

/* Ensure the dynamic array has enough space for at least 'needed' elements. */
static void ensure_capacity(size_t needed)
{
    if (needed <= seq_cap)
        return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 32;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (seq) {
        memcpy(new_arr, seq, seq_len * sizeof(seq_t));
        free(seq);
    }
    seq = new_arr;
    seq_cap = new_cap;
    /* keep placeholder at position 0 */
    if (seq_len == 1)
        seq[0] = (seq_t)0;
}

/* Test whether a candidate integer satisfies the sequence conditions. */
static int is_valid(seq_t m)
{
    for (int base = 2; base <= 10; ++base) {
        unsigned freq[10] = {0};
        uint8_t digits[128] = {0};
        size_t len = 0;
        seq_t val = m;
        /* extract digits in the current base */
        while (val > 0) {
            unsigned dig = (unsigned)(val % base);
            freq[dig]++;
            digits[len++] = (uint8_t)dig;
            val /= base;
        }
        /* compute min and max frequencies among digits that actually appear */
        int mincnt = -1, maxcnt = -1;
        for (int d = 0; d < base; ++d) {
            if (freq[d] > 0) {
                if (mincnt == -1 || freq[d] < mincnt)
                    mincnt = freq[d];
                if (maxcnt == -1 || freq[d] > maxcnt)
                    maxcnt = freq[d];
            }
        }
        if (maxcnt - mincnt > 1)
            return 0;  /* condition (1) fails */

        /* condition (2): reject representation of a single repeated digit */
        if (len > 1) {
            int all_eq = 1;
            uint8_t first = digits[0];
            for (size_t i = 1; i < len; ++i) {
                if (digits[i] != first) {
                    all_eq = 0;
                    break;
                }
            }
            if (all_eq)  /* all digits identical and length >= 2 */
                return 0;
        }
    }
    return 1;  /* passes all checks */
}

/* Generate terms until we have at least n terms (seq_len-1 >= n). */
static void generate_until(idx_t n)
{
    while (seq_len <= (size_t)n) {
        /* search for the next valid integer */
        while (!is_valid(next_candidate))
            ++next_candidate;

        /* store the found term */
        ensure_capacity(seq_len + 1);
        seq[seq_len] = next_candidate;
        ++seq_len;
        ++next_candidate;
    }
}

/* Public function required by the harness. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;             /* undefined for n < 1, return 0 as safe */
    generate_until(n);
    return seq[n];
}
