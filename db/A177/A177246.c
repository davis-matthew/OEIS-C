/*
 * A177246  Fibonacci numbers whose decimal expansion does not contain any
 *          digit 4.
 * https://oeis.org/A177246
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 18, u32 n <= 26, u64 n <= 38, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A177246: a(n) = the nth Fibonacci number whose decimal representation contains no digit 4
   (computed by sequential search) */

#include <stdbool.h>

/* Helper: true if x contains the digit '4' */
static inline bool has_digit4(seq_t x) {
    while (x > (seq_t)0) {
        if ((x % (seq_t)10) == (seq_t)4) return true;
        x /= (seq_t)10;
    }
    return false;
}

seq_t a(idx_t n) {
    if (n < (idx_t)1) return (seq_t)0;   /* undefined for n < 1 */

    seq_t prev = (seq_t)1;   /* F1 */
    seq_t curr = (seq_t)1;   /* F2 */
    idx_t count = 0;

    /* check F1 */
    if (!has_digit4(prev)) {
        count++;
        if (count == n) return prev;
    }
    /* check F2 */
    if (!has_digit4(curr)) {
        count++;
        if (count == n) return curr;
    }

    /* generate subsequent Fibonacci numbers */
    while (count < n) {
        seq_t next = prev + curr;   /* F(k+1) = F(k-1) + F(k) */
        prev = curr;
        curr = next;
        if (!has_digit4(curr)) {
            count++;
            if (count == n) return curr;
        }
    }
    return (seq_t)0;   /* unreachable */
}
