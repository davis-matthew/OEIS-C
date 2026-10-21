/*
 * A082288  n>1 appears bigomega(n) times, where bigomega(n)=A001222(n) is the
 *          number of prime factors of n (with repetition), a(1)=1.
 * https://oeis.org/A082288
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A082288: a(1)=1, for n>1 the integer n appears
 * exactly bigomega(n) times (bigomega = total number of prime
 * factors with multiplicity).  The function returns the n-th term
 * of this sequence.
 *
 * Implementation uses a simple incremental generator that
 * stores previously generated terms in a dynamic array so that
 * subsequent calls are fast.  The helper `bigomega` is provided
 * by the utils library.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/bigomega.h"

static seq_t *seq = NULL;        /* dynamic array holding the sequence */
static size_t seq_cap = 0;       /* capacity of the array */
static size_t seq_len = 0;       /* number of terms currently stored */
static idx_t next_val = 2;      /* next integer whose repetitions need to be added */

/* Ensure that the array has at least `need` capacity */
static void grow_seq(size_t need)
{
    if (seq_cap >= need) return;
    size_t newcap = seq_cap ? seq_cap * 2 : 128;
    while (seq_cap < need) {
        newcap <<= 1;
    }
    seq = (seq_t *)realloc(seq, newcap * sizeof(seq_t));
    seq_cap = newcap;
}

/* Generate terms until the array holds at least `n` elements */
static void extend_to(idx_t n)
{
    if (seq == NULL) {
        /* initialisation: put the value 1 at position 0 (a(1)=1) */
        seq_cap = 128;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        seq[0] = (seq_t)1;
        seq_len = 1;
    }
    while (seq_len < (size_t)n) {
        /* for the current integer, add `omega` copies of its value */
        seq_t omega = bigomega(next_val);
        size_t copies = (size_t)omega;
        /* resize array if necessary */
        grow_seq(seq_len + copies);
        /* add the copies */
        for (size_t i = 0; i < copies; ++i) {
            seq[seq_len++] = (seq_t)next_val;
        }
        ++next_val;
    }
}

/* Public function required by the test harness */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* undefined per the offset, but safe */
    extend_to(n);
    return seq[n - 1];                     /* array is 0‑based, indices start at 1 */
}
