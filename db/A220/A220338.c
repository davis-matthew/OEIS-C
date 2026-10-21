/*
 * A220338  A modified Engel expansion for 8*sqrt(6) - 19.
 * https://oeis.org/A220338
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(3k+1)=2; a(3k+2)=2*(a(3k-1)^2-2*a(3k-1)+1);
 *            a(3k+3)=2*a(3k+2)-2 with a(1)=2, a(2)=6, a(3)=10
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 13, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Lazy initialization of static caches */
    static seq_t *a2 = NULL;      /* a(3k+2) terms, indexed by k>=0 */
    static seq_t *a3 = NULL;      /* a(3k+3) terms, indexed by k>=0 */
    static size_t a2_size = 0, a2_cap = 0;
    static size_t a3_size = 0, a3_cap = 0;
    static int initialized = 0;

    if (!initialized) {
        /* a2[0] = a(2) = 6 */
        a2_cap = 4;
        a2 = (seq_t *)malloc(a2_cap * sizeof(seq_t));
        a2[0] = (seq_t)6;
        a2_size = 1;

        /* a3[0] = a(3) = 10 */
        a3_cap = 4;
        a3 = (seq_t *)malloc(a3_cap * sizeof(seq_t));
        a3[0] = (seq_t)10;
        a3_size = 1;
        initialized = 1;
    }

    if (n == 1)
        return (seq_t)2;

    idx_t r = n % 3;
    if (r == 1)   /* terms of the form 3k+1 are all 2 */
        return (seq_t)2;

    if (r == 2) {  /* a(3k+2) */
        size_t idx = (size_t)((n - 2) / 3);  /* k value */

        /* grow a2 and a3 up to idx */
        while (a2_size <= idx) {
            seq_t prev = a2[a2_size - 1];
            seq_t next = (seq_t)2 * ((prev * prev) - (seq_t)2 * prev + (seq_t)1);

            /* resize a2 if needed */
            if (a2_size == a2_cap) {
                a2_cap = (a2_cap * 2) > 0 ? (a2_cap * 2) : 4;
                a2 = (seq_t *)realloc(a2, a2_cap * sizeof(seq_t));
            }
            a2[a2_size++] = next;

            /* compute corresponding a3 term */
            seq_t next_a3 = (seq_t)2 * next - (seq_t)2;
            if (a3_size == a3_cap) {
                a3_cap = (a3_cap * 2) > 0 ? (a3_cap * 2) : 4;
                a3 = (seq_t *)realloc(a3, a3_cap * sizeof(seq_t));
            }
            a3[a3_size++] = next_a3;
        }
        return a2[idx];
    } else {        /* r == 0, a(3k) = a3[k-1] */
        size_t idx = (size_t)(n / 3) - 1;   /* k-1 value */

        /* grow a2 and a3 up to idx+1 (since a3 indices correspond to a2 indices) */
        while (a3_size <= idx) {
            seq_t prev = a2[a2_size - 1];
            seq_t next = (seq_t)2 * ((prev * prev) - (seq_t)2 * prev + (seq_t)1);

            if (a2_size == a2_cap) {
                a2_cap = (a2_cap * 2) > 0 ? (a2_cap * 2) : 4;
                a2 = (seq_t *)realloc(a2, a2_cap * sizeof(seq_t));
            }
            a2[a2_size++] = next;

            seq_t next_a3 = (seq_t)2 * next - (seq_t)2;
            if (a3_size == a3_cap) {
                a3_cap = (a3_cap * 2) > 0 ? (a3_cap * 2) : 4;
                a3 = (seq_t *)realloc(a3, a3_cap * sizeof(seq_t));
            }
            a3[a3_size++] = next_a3;
        }
        return a3[idx];
    }
}
