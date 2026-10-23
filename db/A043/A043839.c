/*
 * A043839  Numbers k such that number of runs in the base 4 representation of
 *          k is congruent to 2 mod 6.
 * https://oeis.org/A043839
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = the nth integer k such that the number of runs in
   the base‑4 representation of k is congruent to 2 modulo 6.
   The function builds the sequence on demand by scanning natural
   numbers and testing the run condition.  It can be called in any
   order and remains fast for n up to at least 10,000. */
#include <stdlib.h>
#include <stddef.h>

static seq_t *seq_cache = NULL;        /* array of found terms   */
static size_t seq_len   = 0;           /* number of terms stored*/
static size_t seq_cap   = 0;           /* allocated capacity    */
static idx_t  next_k    = 1;           /* next integer to test  */

static unsigned int count_runs_base4(idx_t k)
{
    unsigned int runs = 0;
    int last_digit = -1;
    while (k > 0) {
        int digit = (int)(k % 4);      /* k % 4 returns seq_t, cast to int */
        if (digit != last_digit) {
            ++runs;
            last_digit = digit;
        }
        k /= 4;
    }
    return runs;
}

seq_t a(idx_t n)
{
    /* Build terms until we have at least n of them */
    while (seq_len < n) {
        unsigned int runs = count_runs_base4(next_k);
        if (runs % 6 == 2) {
            /* store the value */
            if (seq_len == seq_cap) {
                size_t new_cap = seq_cap ? seq_cap * 2 : 1;
                seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
                seq_cap = new_cap;
            }
            seq_cache[seq_len++] = (seq_t)next_k;
        }
        ++next_k;
    }

    return seq_cache[n - 1];
}
