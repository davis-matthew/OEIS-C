/*
 * A072137  Length of the preperiodic part of the 'Reverse and Subtract'
 *          trajectory of n.
 * https://oeis.org/A072137
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn, nice
 * Formula:   not stated
 * Range:     u8 n <= 102, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   reverse
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the length of the preperiodic part of the Reverse‑and‑Subtract trajectory of n
   (OEIS A072137).  The trajectory is defined by
        x₀ = n
        x_{k+1} = |x_k – rev(x_k)|
   where rev(x) is the decimal digit reversal.  The return value is the first
   index i such that x_i == x_j for some j < i (i.e. the length of the
   preperiod).  The algorithm uses Floyd’s cycle‑finding method so it works
   for arbitrary‑precision seq_t and requires only O(1) additional memory. */
#include <stdlib.h>
#include "../utils/reverse.h"

seq_t a(idx_t n)
{
    /* initial state */
    seq_t tortoise = (seq_t)n;
    seq_t hare     = (seq_t)n;

    /* advance tortoise by one step */
    seq_t rev_t = reverse(tortoise);
    tortoise = (tortoise >= rev_t) ? (tortoise - rev_t) : (rev_t - tortoise);

    /* advance hare by two steps until the two pointers meet */
    while (1) {
        /* hare -> next step */
        seq_t rev_h = reverse(hare);
        hare = (hare >= rev_h) ? (hare - rev_h) : (rev_h - hare);

        /* hare -> second step */
        rev_h = reverse(hare);
        hare = (hare >= rev_h) ? (hare - rev_h) : (rev_h - hare);

        if (tortoise == hare)
            break;                  /* cycle found */

        /* tortoise -> next step */
        rev_t = reverse(tortoise);
        tortoise = (tortoise >= rev_t) ? (tortoise - rev_t) : (rev_t - tortoise);
    }

    /* find the start of the cycle (mu) */
    seq_t ptr1 = (seq_t)n;
    seq_t ptr2 = tortoise;            /* tortoise is now on the cycle */
    size_t mu  = 0;

    while (ptr1 != ptr2) {
        rev_t = reverse(ptr1);
        ptr1 = (ptr1 >= rev_t) ? (ptr1 - rev_t) : (rev_t - ptr1);

        rev_t = reverse(ptr2);
        ptr2 = (ptr2 >= rev_t) ? (ptr2 - rev_t) : (rev_t - ptr2);

        mu++;
    }

    return (seq_t)mu;
}
