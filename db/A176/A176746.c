/*
 * A176746  The Fibonacci sequence (A000045) and the integers that cannot be
 *          represented as a sum of two earlier terms in the sequence.
 * https://oeis.org/A176746
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A176746 – The Fibonacci sequence and the integers that cannot be represented
 * as a sum of two earlier terms in the sequence.
 * The function a(idx_t n) returns the nth element of this sequence
 * (with a(0)=0).
 * Approach: lazily generate the sequence, keep it in a dynamic array.
 * At each step examine the next integer m:
 *   – If m is the next Fibonacci number, include it.
 *   – Else if m cannot be expressed as a sum of two (possibly equal)
 *     earlier terms, include it.
 * The algorithm is O(n²) in the worst case but the average
 * per term work is linear, comfortably handling n≈10000.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* Static state shared by all calls. */
static seq_t *seq = NULL;          /* dynamic array of sequence terms   */
static size_t seq_cap = 0;         /* capacity of seq                   */
static size_t seq_count = 0;       /* number of terms currently stored  */

static seq_t fib_prev = 0;         /* F(k-1) */
static seq_t fib_curr = 1;         /* F(k)   */
static seq_t fib_next = 1;         /* F(k+1) */
static seq_t next_candidate = 0;   /* next integer to examine          */
static bool initialized = false;  /* start‑up guard                    */

/* Ensure seq holds at least one more element. */
static void grow_seq(void)
{
    if (seq_count < seq_cap)
        return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 8;
    seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    seq = tmp;
    seq_cap = new_cap;
}

/* Push a new value onto the sequence. */
static void push_seq(seq_t v)
{
    grow_seq();
    seq[seq_count++] = v;
}

/* One‑time initialization of the sequence and Fibonacci state. */
static void init_sequence(void)
{
    seq_cap = 8;
    seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
    seq_count = 0;

    /* First three terms: 0, 1, 1 */
    seq[0] = (seq_t)0;
    seq[1] = (seq_t)1;
    seq[2] = (seq_t)1;
    seq_count = 3;

    /* Fibonacci state after the second “1” */
    fib_prev = (seq_t)1;   /* F2 */
    fib_curr = (seq_t)2;   /* F3 */
    fib_next = (seq_t)3;   /* F4 */

    /* Next integer to evaluate */
    next_candidate = (seq_t)2;

    initialized = true;
}

/* Generate the sequence up to at least index target_idx (inclusive). */
static void generate_to(idx_t target_idx)
{
    if (!initialized)
        init_sequence();

    while (seq_count <= (size_t)target_idx) {
        seq_t cand = next_candidate;

        /* Advance Fibonacci state until fib_curr >= cand. */
        while (fib_next <= cand) {
            fib_prev = fib_curr;
            fib_curr = fib_next;
            fib_next = fib_prev + fib_curr;
        }

        bool added = false;

        if (cand == fib_curr) {                 /* Fibonacci number */
            added = true;
            fib_prev = fib_curr;
            fib_curr = fib_next;
            fib_next = fib_prev + fib_curr;
        } else {                                /* Non‑Fibonacci */
            /* Test if cand can be expressed as a sum of two earlier terms. */
            bool can_sum = false;
            size_t i = 0;
            size_t j = seq_count - 1;
            while (i <= j) {
                /* Skip terms larger than cand. */
                if (seq[j] > cand) { j--; continue; }
                seq_t remaining = cand - seq[j];
                if (seq[i] > remaining) {       /* sum > cand */
                    j--;
                } else if (seq[i] < remaining) { /* sum < cand */
                    i++;
                } else {                         /* exact match */
                    can_sum = true;
                    break;
                }
            }
            if (!can_sum)
                added = true;
        }

        if (added)
            push_seq(cand);

        ++next_candidate;  /* advance to the next integer */
    }
}

/* Public function required by the framework. */
seq_t a(idx_t n)
{
    generate_to(n);
    return seq[n];
}
