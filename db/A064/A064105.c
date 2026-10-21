/*
 * A064105  2nd column of 3rd-order Zeckendorf array A136189.
 * https://oeis.org/A064105
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 9632, u32 n <= 9632, u64 n <= 9632,
 *            u128 n <= 9632, big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: generate terms by testing natural numbers until the
   required index is reached.  For a number m we compute its
   3‑rd‑order Zeckendorf representation (using the sequence
   2,3,4,6,9,13,…) and check whether the term 2 is used.  Every
   time the representation contains 2 the number m is appended to
   the list of terms a(1), a(2), … .  The sequence is indexed from
   1 as in OEIS.  The algorithm is O(n) for the first n terms. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Helpers provided by the build. */
#include "../utils/seq_realloc.h"  /* seq_realloc(seq_t*,size_t,size_t) */

/* Internal state: the cached Zeckendorf term list and the
   generated a‑sequence. */
static seq_t *T      = NULL;    /* Zeckendorf terms                         */
static size_t T_cap = 0;
static idx_t  T_len = 0;

static seq_t *seq    = NULL;    /* a(n) values                               */
static size_t seq_cap = 0;
static idx_t  seq_len = 0;

/* Ensure T contains all terms <= limit.  The recurrence is
   T(k) = T(k-1) + T(k-3).  */
static void ensure_T_up_to(seq_t limit)
{
    /* initialise the first three terms if necessary */
    if (T_len == 0) {
        T_cap = 4;
        T = seq_realloc(NULL, 0, T_cap);
        T[0] = (seq_t)2;
        T[1] = (seq_t)3;
        T[2] = (seq_t)4;
        T_len = 3;
    }

    /* extend until the last term is >= limit */
    while (T_len > 0 && T[T_len - 1] < limit) {
        seq_t next = T[T_len - 1] + T[T_len - 3];
        if (T_len == T_cap) {
            size_t new_cap = T_cap ? T_cap * 2 : 16;
            T = seq_realloc(T, T_cap, new_cap);
            T_cap = new_cap;
        }
        T[T_len++] = next;
    }
}

/* Test whether the 3rd‑order Zeckendorf representation of m
   uses the term 2 (index 0).  The greedy algorithm respects
   the rule that no two used indices may be consecutive. */
static bool has2_representation(seq_t m)
{
    if (m < (seq_t)2) return false;
    ensure_T_up_to(m);

    bool prev_used = false;
    bool used2     = false;

    /* iterate over terms from largest to smallest */
    for (idx_t i = T_len; i-- > 0;) {
        if (!prev_used && T[i] <= m) {
            m -= T[i];
            prev_used = true;
            if (i == 0) used2 = true;
        } else {
            prev_used = false;
        }
        if (m == 0) break;
    }
    return (m == 0) && used2;
}

/* Append a value to the cached a‑sequence. */
static void push_a(idx_t val)
{
    if (seq_len == seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 16;
        seq = seq_realloc(seq, seq_cap, new_cap);
        seq_cap = new_cap;
    }
    seq[seq_len++] = (seq_t)val;
}

/* Public entry point for the sequence. */
seq_t a(idx_t n)
{
    static idx_t last_scanned = 1;          /* last natural number examined */

    while (seq_len < n) {
        ++last_scanned;
        if (has2_representation((seq_t)last_scanned))
            push_a(last_scanned);
    }
    return seq[n - 1];
}
