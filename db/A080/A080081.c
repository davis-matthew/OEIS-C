/*
 * A080081  Beatty sequence for (3+sqrt(13))/2.
 * https://oeis.org/A080081
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 77, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A080081: Beatty sequence for (3+sqrt(13))/2
   Recurrence: a(1)=3;  
   for n>1, a(n+1)=a(n)+4 if n is already in the sequence,
                 a(n)+3 otherwise.  
   The code builds the sequence on demand, keeping the computed
   terms in a dynamic array and using a pointer to test membership
   of the current index.  It works for any seq_t (8,16,32,64,128
   bits or arbitrary precision) and any idx_t (uint64_t). */
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

static seq_t *seq_arr = NULL;   /* storage for a(1)..a(k) */
static idx_t  seq_len = 0;      /* number of terms stored      */
static idx_t  member_ptr = 0;   /* current position for membership test */
static size_t seq_cap = 0;      /* allocated capacity           */

/* Ensure on‑demand capacity for at least 'min' elements. */
static void ensure_seq_cap(idx_t min)
{
    size_t need = (size_t)min;
    if (seq_cap == 0) {
        seq_cap = 64;
        seq_arr = (seq_t *)malloc(seq_cap * sizeof(seq_t));
    }
    while (seq_cap < need) {
        seq_cap <<= 1;
        seq_arr = (seq_t *)realloc(seq_arr, seq_cap * sizeof(seq_t));
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset starts at 1; n==0 is undefined */

    /* Initialise on first call */
    if (seq_len == 0) {
        ensure_seq_cap(1);
        seq_arr[0] = (seq_t)3;   /* a(1) = 3 */
        seq_len   = 1;
        member_ptr = 0;
    }

    /* Grow the sequence until it contains a(n) */
    while (seq_len < n) {
        idx_t i = seq_len;        /* previous index (n-1) */

        /* Advance member_ptr to the first term >= i */
        while (member_ptr < seq_len &&
               (idx_t)seq_arr[member_ptr] < i)
            ++member_ptr;

        bool present = (member_ptr < seq_len &&
                        (idx_t)seq_arr[member_ptr] == i);

        seq_t inc   = present ? (seq_t)4 : (seq_t)3;
        seq_t next  = seq_arr[seq_len - 1] + inc;

        ensure_seq_cap(seq_len + 1);
        seq_arr[seq_len] = next;
        ++seq_len;
    }

    return seq_arr[n - 1];
}
