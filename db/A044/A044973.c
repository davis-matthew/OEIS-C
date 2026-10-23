/*
 * A044973  Numbers whose base-3 representation contains exactly one 0 and
 *          three 1's.
 * https://oeis.org/A044973
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 1638, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A044973: numbers whose base‑3 representation contains exactly one 0 and three 1's */

/* Standard headers required for memory allocation */
#include <stdlib.h>

/* Global static storage for the sequence */
static seq_t *seq      = NULL;
static size_t  seq_len = 0;
static size_t  seq_cap = 0;
static idx_t   next_len = 4;      /* next base‑3 length to generate */

/* Ensure the sequence array can hold at least "need" elements. */
static void ensure_capacity(size_t need)
{
    if (need <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    while (new_cap < need) new_cap <<= 1;
    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* Append a value to the global sequence, enlarging the array if necessary. */
static void append_val(seq_t v)
{
    ensure_capacity(seq_len + 1);
    seq[seq_len++] = v;
}

/* Recursive descent that builds all base‑3 strings of a given length L
   starting with a first digit fd (1 or 2).  It places exactly one 0 and
   the required number of 1's among the remaining positions. */
static void gen_len_rec(idx_t pos, int remZero, int remOnes,
                        seq_t val, idx_t L)
{
    if (pos == L) {
        if (remZero == 0 && remOnes == 0) append_val(val);
        return;
    }
    /* Try digit 0 if we still need to place the single zero. */
    if (remZero) {
        gen_len_rec(pos + 1, remZero - 1, remOnes,
                    (seq_t)val * (seq_t)3 + (seq_t)0, L);
    }
    /* Try digit 1 if we still need to place one of the three ones. */
    if (remOnes) {
        gen_len_rec(pos + 1, remZero, remOnes - 1,
                    (seq_t)val * (seq_t)3 + (seq_t)1, L);
    }
    /* 2 is always allowed. */
    gen_len_rec(pos + 1, remZero, remOnes,
                (seq_t)val * (seq_t)3 + (seq_t)2, L);
}

/* Generate all numbers of base‑3 length L with the required digit
   counts, prefixed by a first digit fd (1 or 2). */
static void generate_length(idx_t L, uint8_t fd)
{
    seq_t initial = fd;
    int remZero = 1;                 /* still need one zero */
    int remOnes = (fd == 1) ? 2 : 3; /* three ones total, minus first digit if 1 */
    gen_len_rec(1, remZero, remOnes, initial, L);
}

/* Return the n-th term (n ≥ 1) of the sequence. */
seq_t a(idx_t n)
{
    while (seq_len < n) {
        idx_t L = next_len;
        /* Numbers starting with 1 are valid for all L ≥ 4. */
        if (L >= 4) generate_length(L, 1);
        /* Numbers starting with 2 require an extra digit. */
        if (L >= 5) generate_length(L, 2);
        ++next_len;
    }
    return seq[n - 1];
}
