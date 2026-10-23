/*
 * A117722  a(n) = A000045(A003622(n)).
 * https://oeis.org/A117722
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, less, nonn
 * Formula:   a(n) = A000045(A003622(n))
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes a(n) by generating the Fibonacci word and storing
    the positions of the '1' characters.  The nth such position
    is used as the index to the Fibonacci sequence. */

#include <stdlib.h>
#include <string.h>
#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* Static state: Fibonacci word buffers and 1‑position cache */
    static char *w0 = NULL;        /* previous word F_{k-1} */
    static size_t len0 = 0;        /* length of w0              */
    static char *w1 = NULL;        /* current word F_k          */
    static size_t len1 = 0;        /* length of w1              */
    static idx_t *pos1 = NULL;     /* 1‑positions, 0‑based      */
    static size_t pos1_len = 0;    /* number of stored positions */
    static size_t pos1_cap = 0;    /* capacity of pos1 array    */

    /* One‑time initialization */
    if (!w0) {
        w0 = (char *)malloc(1);    /* F_0 = "0" */
        w0[0] = '0';
        len0 = 1;

        w1 = (char *)malloc(2);    /* F_1 = "01" */
        w1[0] = '0';
        w1[1] = '1';
        len1 = 2;

        pos1_cap = 16;
        pos1 = (idx_t *)malloc(pos1_cap * sizeof(idx_t));
        pos1_len = 0;
        /* First occurrence of '1' is at index 1 */
        pos1[pos1_len++] = 1;
    }

    /* Generate words until we have at least n+1 stored '1' positions */
    while (pos1_len <= n) {
        size_t len_new = len1 + len0;
        char *wnew = (char *)malloc(len_new);

        memcpy(wnew, w1, len1);          /* wnew = w1 || w0 */
        memcpy(wnew + len1, w0, len0);

        /* Record positions of '1's coming from the older word w0.
           They are offset by len1 in the new word. */
        for (size_t i = 0; i < len0; ++i) {
            if (w0[i] == '1') {
                if (pos1_len == pos1_cap) {
                    pos1_cap *= 2;
                    pos1 = (idx_t *)realloc(pos1,
                                            pos1_cap * sizeof(idx_t));
                }
                pos1[pos1_len++] = (idx_t)(i + len1);
            }
        }

        /* Advance the Fibonacci word buffers */
        free(w0);
        w0 = w1;
        len0 = len1;
        w1 = wnew;
        len1 = len_new;
    }

    /* pos1[n] is the n‑th position (0‑based) of a '1' in the Fibonacci word.
       Use it as the index to the Fibonacci sequence. */
    return fib((idx_t)pos1[n]);
}
