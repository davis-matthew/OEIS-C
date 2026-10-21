/*
 * A081549  a(1) = 1; for n > 1, a(n) > a(n-1) is the smallest number such
 *          that the concatenation a(1)a(2)a(3)... forms a cyclic
 *          concatenation of 123456789 (of nonzero digits).
 * https://oeis.org/A081549
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 20, u32 n <= 28, u64 n <= 60, u128 n <= 115,
 *            big any n
 * Helpers:   digit_at
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence: A081549
 * Definition: a(1)=1. For n>1, a(n) is the smallest integer greater than a(n-1)
 * that can be read from the next consecutive digits of the infinite string
 * 123456789123456789... (digits 1–9 repeat). The concatenation
 * a(1)a(2)...a(n) must always be a prefix of this cyclic string.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/digit_at.h"

/* Dynamic storage for the sequence terms */
static seq_t *seq_terms = NULL;
static size_t seq_len = 0;   /* number of terms stored */
static size_t seq_cap = 0;   /* allocated capacity */

/* Position in the infinite digit string (1‑based).  The next digit to
 * read is digit_at(pos).  After consuming a digit, pos is incremented. */
static idx_t digit_pos = 1;

/* The last term generated */
static seq_t prev_term = 0;

/* Grow the dynamic array if required */
static void ensure_capacity(void)
{
    if (seq_len == seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 8;
        seq_t *tmp = (seq_t *)realloc(seq_terms, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);                /* allocation failure */
        seq_terms = tmp;
        seq_cap   = new_cap;
    }
}

/* Generate terms up to index n (1‑based) if not already generated */
static void generate_up_to(idx_t n)
{
    /* Initialisation of the first term */
    if (seq_len == 0) {
        ensure_capacity();
        seq_terms[0] = (seq_t)1;
        seq_len = 1;
        prev_term = (seq_t)1;
        digit_pos = 2;                     /* next digit to read */
    }

    /* Generate subsequent terms until seq_len == n */
    while ((idx_t)seq_len < n) {
        seq_t cur = (seq_t)0;
        while (true) {
            int d = digit_at(digit_pos);
            digit_pos++;
            cur = cur * 10 + (seq_t)d;
            if (cur > prev_term)
                break;
        }
        ensure_capacity();
        seq_terms[seq_len++] = cur;
        prev_term = cur;
    }
}

seq_t a(idx_t n)
{
    generate_up_to(n);
    return seq_terms[n - 1];
}
