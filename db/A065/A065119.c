/*
 * A065119  Numbers k such that the k-th cyclotomic polynomial is a trinomial.
 * https://oeis.org/A065119
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 78, u32 n <= 317, u64 n <= 1280,
 *            u128 n <= 5144, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
/// Sequence: numbers k such that the k‑th cyclotomic polynomial is a trinomial.
/// a(n) is the nth integer of the form 2^a · 3^b with a ≥ 0 and b > 0.
/// Equivalently, the nth 3‑smooth number that is divisible by 3.
*/
#include <stdlib.h>
#include <stdint.h>

/* buffers containing all generated 3‑smooth numbers (array “all”)
   and the subset that is a multiple of 3 (array “tri”).            */
static seq_t *all = NULL; /* all 3‑smooth numbers (including 1) */
static seq_t *tri = NULL; /* only those divisible by 3          */

/* lengths and capacities of the two dynamic arrays             */
static size_t all_len   = 0, all_cap   = 0;
static size_t tri_len   = 0, tri_cap   = 0;

/* two pointers used by the classic Hamming‑number generator   */
static size_t i2 = 0, i3 = 0;

/* Ensure that a dynamic array of type seq_t has the requested
   capacity.  The function is written in a type‑agnostic way, so
   it can be used for the arbitrary‑precision seq_t as well as for
   ordinary integer types. */
static void grow(void **ptr, size_t *len, size_t *cap, size_t needed,
                 size_t elem_size)
{
    if (*cap < needed) {
        size_t new_cap = (*cap == 0) ? 16 : (*cap << 1);
        while (new_cap < needed) new_cap <<= 1;
        void *tmp = realloc(*ptr, new_cap * elem_size);
        if (!tmp) abort();                    /* out of memory */
        *ptr = tmp;
        *cap = new_cap;
    }
    *len = needed;                          /* caller guarantees needed */
}

/* Public sequence routine */
seq_t a(idx_t n)
{
    /* Initialise the containers on first call.               */
    if (all == NULL) {
        all_len = 1;
        all_cap = 16;
        all = (seq_t*)calloc(all_cap, sizeof(seq_t)); /* explicit cast for C++ */
        if (!all) abort();
        all[0] = (seq_t)1;          /* the first 3‑smooth number */
    }

    /* Generate numbers until we have at least n multiples of 3.   */
    while (tri_len < (size_t)n) {
        /* Compute the two candidates derived from already generated
           numbers.  The list “all” is strictly increasing, so the
           computed values are also increasing.                     */
        seq_t next2 = (seq_t)((seq_t)all[i2] * (seq_t)2);
        seq_t next3 = (seq_t)((seq_t)all[i3] * (seq_t)3);
        seq_t next  = (next2 < next3) ? next2 : next3;

        /* Advance the pointers that produced this value.            */
        if (next == next2) ++i2;
        if (next == next3) ++i3;

        /* Append the new value to the all‑array.                     */
        grow((void**)&all, &all_len, &all_cap, all_len + 1, sizeof(seq_t));
        all[all_len - 1] = next;

        /* If it is divisible by 3, store it in the tri‑array.      */
        if ((next % (seq_t)3) == (seq_t)0) {
            grow((void**)&tri, &tri_len, &tri_cap, tri_len + 1, sizeof(seq_t));
            tri[tri_len - 1] = next;
        }
    }

    /* The OEIS indices are 1‑based. */
    return tri[(size_t)n - 1];
}
