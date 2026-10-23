/*
 * A130514  To compute the next term, add to n the number t, where t is the
 *          number of [last digit of n]'s used so far in the sequence.
 * https://oeis.org/A130514
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  a(0) = 0.
 * For n > 0: a(n) = a(n‑1) + t, where t is the number of times the
 * last decimal digit of a(n‑1) has appeared in the sequence so far.
 * The sequence is memoised in a static array so that calls with any
 * order of n are answered correctly.
 */
#include <stdlib.h>
#include <stdint.h>

static void add_digit_counts(seq_t x, uint32_t freq[10])
{
    if (x == (seq_t)0) {
        freq[0] += 1;
        return;
    }
    const seq_t ten = (seq_t)10;
    while (x != (seq_t)0) {
        unsigned int d = (unsigned int)(x % ten);
        freq[d] += 1;
        x /= ten;
    }
}

seq_t a(idx_t n)
{
    static seq_t *seq      = NULL;
    static idx_t  seq_len  = 0;      /* number of terms stored (size)      */
    static size_t seq_cap  = 0;      /* capacity of the allocated array    */
    static uint32_t freq[10] = {0};

    /* Initialise on first call */
    if (seq == NULL) {
        seq_cap = 64;
        seq     = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        if (!seq) return (seq_t)0;            /* unlikely: guard */
        seq[0]   = (seq_t)0;
        seq_len  = 1;
        freq[0]  = 1;                         /* '0' from the first term */
    }

    /* Grow the sequence up to the requested index n */
    while ((idx_t)n >= seq_len) {
        /* grow capacity if necessary */
        if (seq_len == seq_cap) {
            seq_cap = seq_cap ? seq_cap * 2 : 64;
            seq_t *tmp = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;        /* guard */
            seq = tmp;
        }

        seq_t prev      = seq[(size_t)seq_len - 1];
        unsigned int ld = (unsigned int)(prev % (seq_t)10);
        idx_t t          = freq[ld];
        seq_t curr       = prev + t;

        seq[(size_t)seq_len] = curr;
        add_digit_counts(curr, freq);
        seq_len++;
    }

    return seq[(size_t)n];
}
