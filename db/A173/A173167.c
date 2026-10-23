/*
 * A173167  Values n such that n and n+1 are both in A037073.
 * https://oeis.org/A173167
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 3, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A173167: a(n) are the lower terms of consecutive pairs in A037073.
   A037073 consists of integers m such that both 18*m*m-1 and 18*m*m+1 are prime.
   The function builds this sequence on demand. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

static seq_t *a_seq = NULL;          /* dynamically grown array holding a(1..size) */
static size_t a_seq_size = 0;        /* current number of elements stored */
static size_t a_seq_capacity = 0;    /* allocated capacity */

static idx_t last_checked = 0;       /* last n examined for membership in A037073 */
static idx_t last_valid = 0;         /* last n that passed the A037073 test */

/* Ensure that a_seq has capacity at least needed. */
static void ensure_capacity(size_t needed)
{
    if (needed <= a_seq_capacity)
        return;
    size_t new_cap = a_seq_capacity ? a_seq_capacity * 2 : 16;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *new_ptr = (seq_t *)realloc(a_seq, new_cap * sizeof(seq_t));
    if (!new_ptr) {
        /* In a contest environment, exiting is acceptable. */
        exit(1);
    }
    a_seq = new_ptr;
    a_seq_capacity = new_cap;
}

/* Test whether m is in A037073. */
static inline bool in_A037073(idx_t m)
{
    seq_t mm = (seq_t)m;
    seq_t v1 = (seq_t)18 * mm * mm - (seq_t)1;
    seq_t v2 = (seq_t)18 * mm * mm + (seq_t)1;
    return is_prime_big(v1) && is_prime_big(v2);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset is 1; a(0) is undefined, return 0 */
    /* Extend the cached sequence until we have at least n terms. */
    while (a_seq_size < (size_t)n) {
        ++last_checked;
        if (in_A037073(last_checked)) {
            if (last_valid != 0 && last_checked == last_valid + (idx_t)1) {
                ensure_capacity(a_seq_size + 1);
                a_seq[a_seq_size++] = (seq_t)last_valid;
            }
            last_valid = last_checked;
        }
    }
    return a_seq[n - 1];
}
