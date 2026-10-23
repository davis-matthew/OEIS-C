/*
 * A033846  Numbers whose prime factors are 2 and 5.
 * https://oeis.org/A033846
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*A003592(n) (the Hamming numbers 2^i*5^j)
 * Range:     u8 n <= 9, u16 n <= 45, u32 n <= 199, u64 n <= 837,
 *            u128 n <= 3437, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* n == 0 is out of the offset, return 0 */
    if (n == 0) return 0;

    /* static storage for the Hamming numbers  h[0] = 1 */
    static seq_t *h = NULL;
    static size_t h_cap = 0;
    static idx_t h_len = 0;
    static idx_t idx2 = 0;  /* next candidate from *2 */
    static idx_t idx5 = 0;  /* next candidate from *5 */

    /* first call: allocate and initialise */
    if (h == NULL) {
        h_cap = 16;
        h = (seq_t *)malloc(h_cap * sizeof(seq_t));
        if (!h) return 0;   /* allocation failure */
        h[0] = (seq_t)1;
        h_len = 1;
        idx2 = 0;
        idx5 = 0;
    }

    /* Grow the array until we have at least n terms */
    while (h_len < n) {
        /* Next candidates */
        seq_t v2 = h[idx2] * (seq_t)2;
        seq_t v5 = h[idx5] * (seq_t)5;
        seq_t v = (v2 < v5) ? v2 : v5;

        /* Ensure capacity */
        if ((size_t)h_len >= h_cap) {
            h_cap = h_cap * 2 + 1;
            seq_t *tmp = (seq_t *)realloc(h, h_cap * sizeof(seq_t));
            if (!tmp) {
                free(h);
                h = NULL;
                return 0;   /* allocation failure */
            }
            h = tmp;
        }

        h[h_len++] = v;   /* store the new Hamming number */

        /* Advance pointers that produced this value */
        if (v == v2) idx2++;
        if (v == v5) idx5++;
    }

    /* a(n) = 10 * h[n-1] (h is 0‑based) */
    return h[n - 1] * (seq_t)10;
}
