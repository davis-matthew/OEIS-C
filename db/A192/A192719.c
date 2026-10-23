/*
 * A192719  Chain of Collatz sequences.
 * https://oeis.org/A192719
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   A192719 – chain of Collatz sequences. Each Collatz sequence
 *            begins with the smallest positive integer not yet appearing in
 *            any previous sequence. The values are emitted in order, and this
 *            function returns the n‑th value of that list (1‑based, a(1)=1).
 *            The sequence is built incrementally and cache...
 * Range:     u8 n <= 182, u16 n <= 5540, u32 n <= 5540, u64 n <= 5540,
 *            u128 n <= 5540, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;   /* undefined for n=0 */

    /* static state retained between calls */
    static seq_t *seq      = NULL;   /* array of all emitted terms      */
    static size_t seq_len  = 0;      /* current number of terms stored  */
    static size_t seq_cap  = 0;      /* current allocated capacity      */
    static idx_t  next_start = 1;    /* next candidate integer to test  */

    /* generate terms until we have at least n of them */
    while (seq_len < (size_t)n) {
        /* find the smallest positive integer that has not yet appeared */
        idx_t start = next_start;
        for (;;) {
            bool present = false;
            for (size_t i = 0; i < seq_len; ++i) {
                if (seq[i] == (seq_t)start) { present = true; break; }
            }
            if (!present) break;    /* start is the desired integer */
            ++start;
        }
        next_start = start + 1;      /* next search will begin above this value */

        /* append the Collatz trajectory starting at 'start' */
        seq_t x = (seq_t)start;
        for (;;) {
            /* grow the array if necessary */
            if (seq_len == seq_cap) {
                size_t new_cap = seq_cap ? seq_cap * 2 : 16;
                seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
                seq_cap = new_cap;
            }
            seq[seq_len++] = x;     /* store current value */

            if (x == (seq_t)1) break;   /* reached the end of the trajectory */

            /* next Collatz step: even -> x/2, odd -> 3x+1 */
            if ((x & (seq_t)1) == (seq_t)0) {
                x = x / (seq_t)2;
            } else {
                x = (seq_t)3 * x + (seq_t)1;
            }
        }
    }

    /* return the requested term (1‑based index) */
    return seq[(size_t)n - 1];
}
