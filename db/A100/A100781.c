/*
 * A100781  Triangle read by rows: n-th row contains n multiples of n not
 *          occurring earlier all even if n is even else all odd.
 * https://oeis.org/A100781
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 64, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A100781: triangle read by rows – each r‑th row
   contains r multiples of r not appearing earlier; if r is even
   all multiples are considered, if r is odd only odd multiples. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* Global state shared by all calls */
static seq_t *seq_arr      = NULL;   /* sequence values stored so far   */
static size_t seq_len      = 0;      /* number of terms stored          */
static size_t seq_cap      = 0;      /* capacity of seq_arr             */

static unsigned char *used_bits = NULL;   /* bitset of used values   */
static size_t used_bit_count    = 0;      /* number of bits allocated */
static idx_t next_row           = 1;      /* next row to process      */

/* Ensure the bitset can represent the given value. */
static void ensure_used(idx_t val)
{
    if ((size_t)val < used_bit_count) return;   /* already big enough */

    size_t new_bits = used_bit_count ? used_bit_count : 64;
    while ((size_t)val >= new_bits) new_bits <<= 1;

    unsigned char *new_buf = (unsigned char *)realloc(used_bits, new_bits >> 3);
    if (!new_buf) abort();

    /* Initialize newly added bits to 0. */
    size_t old_bytes = used_bit_count >> 3;
    size_t new_bytes = new_bits >> 3;
    if (new_bytes > old_bytes)
        memset(new_buf + old_bytes, 0, new_bytes - old_bytes);

    used_bits = new_buf;
    used_bit_count = new_bits;
}

/* Test if a value has already been used. */
static bool is_used(idx_t val)
{
    if ((size_t)val >= used_bit_count) return false;
    size_t byte = val >> 3;
    uint8_t mask = 1 << (val & 7);
    return (used_bits[byte] & mask) != 0;
}

/* Mark a value as used. */
static void mark_used(idx_t val)
{
    ensure_used(val);
    size_t byte = val >> 3;
    uint8_t mask = 1 << (val & 7);
    used_bits[byte] |= mask;
}

/* Append a new term to the sequence array. */
static void push_seq(seq_t v)
{
    if (seq_len == seq_cap) {
        seq_cap = seq_cap ? seq_cap * 2 : 64;
        seq_arr = (seq_t *)realloc(seq_arr, seq_cap * sizeof(seq_t));
        if (!seq_arr) abort();
    }
    seq_arr[seq_len++] = v;
}

/* Sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset is 1, a(0) undefined */

    /* Grow the sequence until we have at least n terms. */
    while (seq_len < n) {
        idx_t r = next_row++;          /* current row number */

        if ((r & 1ULL) == 0) {         /* even row: all multiples of r */
            idx_t produced = 0;
            idx_t k = 1;
            while (produced < r) {
                seq_t m = (seq_t)r * (seq_t)k;
                idx_t idx = (idx_t)m;  /* type safe cast for used set */
                if (!is_used(idx)) {
                    mark_used(idx);
                    push_seq(m);
                    ++produced;
                }
                ++k;
            }
        } else {                       /* odd row: odd multiples of r */
            idx_t produced = 0;
            idx_t t = 1;
            while (produced < r) {
                seq_t m = (seq_t)r * ((seq_t)(2 * t - 1));
                idx_t idx = (idx_t)m;
                if (!is_used(idx)) {
                    mark_used(idx);
                    push_seq(m);
                    ++produced;
                }
                ++t;
            }
        }
    }

    return seq_arr[n - 1];
}
