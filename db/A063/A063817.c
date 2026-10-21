/*
 * A063817  Growth series for fundamental group of orientable closed surface
 *          of genus 7.
 * https://oeis.org/A063817
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A063817 : Growth series for orientable closed surface of genus 7
   Recurrence derived from the rational generating function
   (1+x)*(1-x^14)/(1-27*x+27*x^14-x^15):
       a(0) = 1
       for n ≥ 1
           a(n) = 27*a(n‑1) – 27*a(n‑14) + a(n‑15)
                  + δ(n)
   where δ(1)=+1, δ(14)=δ(15)=–1, and δ(n)=0 otherwise. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache of previously computed terms */
    static seq_t *buf = NULL;
    static idx_t buf_len = 0;   /* number of terms already computed */
    static idx_t buf_cap = 0;   /* current capacity of buf   */

    /* Ensure capacity for terms up to index n */
    if (n >= buf_cap) {
        size_t new_cap = buf_cap ? (size_t)buf_cap * 2 : 16;
        while ((idx_t)new_cap <= n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(buf, new_cap * sizeof(seq_t));
        if (!tmp) abort();           /* allocation failed */
        buf = tmp;
        buf_cap = (idx_t)new_cap;
    }

    /* Initialise the very first term once */
    if (buf_len == 0) {
        buf[0] = (seq_t)1;
        buf_len = 1;
    }

    /* Compute successive terms up to n if necessary */
    for (idx_t i = buf_len; i <= n; ++i) {
        seq_t val = (seq_t)27 * buf[i - 1];

        if (i >= 14)
            val -= (seq_t)27 * buf[i - 14];
        if (i >= 15)
            val += buf[i - 15];

        /* Handle the special numerator terms */
        if (i == 1)
            val += (seq_t)1;
        if (i == 14 || i == 15)
            val -= (seq_t)1;

        buf[i] = val;
    }

    buf_len = n + 1;
    return buf[n];
}
