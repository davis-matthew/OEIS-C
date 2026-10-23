/*
 * A165303  a(n) is the smallest number not already in the sequence, such that
 *          the concatenation of all a(n) displays the periodic digit string
 *          1, 2, 3, 4, 5 (and repeat).
 * https://oeis.org/A165303
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 21, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence: A165303
 * a(n) is the smallest positive integer not already in the sequence
 * whose decimal digits form the next block of the infinite periodic
 * string 1,2,3,4,5.  The sequence is built incrementally.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Number of digits in the periodic pattern */
#define PATTERN_LEN 5
static const int pattern[PATTERN_LEN] = {1, 2, 3, 4, 5};

/* Global storage for the sequence (1‑based, seq[0] dummy) */
static seq_t *seq = NULL;          /* array of seq_t values */
static size_t seq_len = 0;         /* number of elements stored incl. dummy */
static size_t seq_cap = 0;         /* current capacity of seq */

/* For each starting position modulo 5, remember which lengths have
   already produced a number */
static bool *used_arr[PATTERN_LEN] = {NULL};
static size_t used_cap[PATTERN_LEN] = {0};

/* Current state of the construction process */
static idx_t current_term = 0;     /* number of produced terms (excluding dummy) */
static idx_t current_pos  = 0;     /* index into the pattern of the next digit */

/* Build the integer formed by pattern digits at position 'pos' with
   'len' successive digits.  The result is cast to seq_t. */
static seq_t build_number(idx_t pos, idx_t len)
{
    seq_t v = 0;
    for (idx_t i = 0; i < len; ++i)
        v = v * 10 + (seq_t)pattern[(pos + i) % PATTERN_LEN];
    return v;
}

/* Ensure that the global sequence array has space for at least 'needed'
   elements (including the dummy at seq[0]). */
static void ensure_seq_capacity(size_t needed)
{
    if (seq_cap >= needed)
        return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 1;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!tmp)
        exit(1);
    seq = tmp;
    seq_cap = new_cap;
}

/* Append a new value to the global sequence array. */
static void append_seq(seq_t v)
{
    ensure_seq_capacity(seq_len + 1);
    seq[seq_len++] = v;
}

/* Return the previously stored n‑th term.  Assumes n <= current_term. */
static seq_t get_seq(idx_t n)
{
    return seq[(size_t)n];
}

/* Ensure that used_arr[pos] can index up to 'len'
   (indices 0..len are guaranteed to be allocated). */
static void ensure_used(idx_t pos, idx_t len)
{
    if ((size_t)len <= used_cap[pos])
        return;
    size_t new_cap = used_cap[pos] ? used_cap[pos] << 1 : 1;
    while (new_cap < (size_t)len)
        new_cap <<= 1;
    bool *tmp = (bool *)realloc(used_arr[pos], (new_cap + 1) * sizeof(bool));
    if (!tmp)
        exit(1);
    used_arr[pos] = tmp;
    /* initialize the newly allocated part to false */
    for (size_t i = used_cap[pos] + 1; i <= new_cap; ++i)
        tmp[i] = false;
    used_cap[pos] = new_cap;
}

/* Public function required by the framework */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;          /* undefined for n < 1 */

    /* initialise storage on the first call */
    if (!seq) {
        ensure_seq_capacity(1);
        seq[0] = 0;        /* dummy element for 1‑based indexing */
        seq_len = 1;       /* seq[0] now exists */
    }

    /* generate terms up to n if necessary */
    while (current_term < n) {
        idx_t pos = current_pos;
        idx_t len = 1;
        /* search for the smallest unused number starting at position pos */
        while (1) {
            ensure_used(pos, len);
            if (!used_arr[pos][len]) {
                /* found a new number */
                seq_t val = build_number(pos, len);
                used_arr[pos][len] = true;
                current_pos = (pos + len) % PATTERN_LEN;
                append_seq(val);
                ++current_term;
                break;
            }
            ++len;
        }
    }

    return get_seq(n);
}
