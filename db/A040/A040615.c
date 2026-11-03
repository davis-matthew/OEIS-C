/*
 * A040615  Continued fraction for sqrt(641).
 * https://oeis.org/A040615
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Validated: partial, pass, 91 terms through n = 90 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n-th coefficient of the continued
   fraction expansion of sqrt(641).  The coefficients are periodic
   with period 21, which is obtained by the standard algorithm for
   periodic simple continued fractions. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

static seq_t *alloc_seq_array(size_t n)
{
    return mem_new(seq_t, n);
}

/* Helper to grow a sequence array of type seq_t. */
static void grow_seq_array(seq_t **arr, size_t *size, size_t new_len)
{
    if (*size >= new_len) return;
    size_t new_cap = (*size > 0) ? (*size << 1) : 16;
    while (new_cap < new_len) new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, *arr, new_cap);
    if (!tmp) abort();
    *arr = tmp;
    *size = new_cap;
}

static seq_t *coeff = NULL;        /* array of computed coefficients */
static size_t size = 0;            /* number of coefficients currently stored */
static size_t cap  = 0;            /* allocated size of coeff array */

static seq_t m = 0;                /* recurrence variable m */
static seq_t d = 1;                /* recurrence variable d */
static seq_t a_current = 0;        /* current coefficient a */

static bool initialized = false;   /* have we done the first allocation */

static void ensure_prepared(idx_t n) {
    /* Initialise storage and the recurrence on first use. */
    if (!initialized) {
        initialized = true;
    }

    /* The algorithm below generates the continued‑fraction
       coefficients in order, stopping when we have the n‑th term. */
    while (size <= (size_t)n) {
        if (size == 0) {                     /* first coefficient a0=25   */
            if (coeff == NULL) {
                cap = 1;
                coeff = alloc_seq_array(cap);
            }
            coeff[0] = (seq_t)25;
            a_current = (seq_t)25;
            m = 0;
            d = 1;
            size = 1;                        /* we just put a0   */
        } else {
            if (size >= cap) {
                /* grow the array; grow_seq_array ensures the size is
                   at least new_len.                                          */
                grow_seq_array(&coeff, &cap, size + 1);
            }
            /* Recurrence for periodic continued fractions of sqrt(D)  */
            m = d * a_current - m;
            d = ((seq_t)641 - m * m) / d;
            a_current = ((seq_t)25 + m) / d;
            coeff[size] = a_current;
            size++;
        }
    }
}

seq_t a(idx_t n) {
    ensure_prepared(n);
    return coeff[(size_t)n];
}
