/*
 * A117540  Scrambled version of the first 8 terms of the Fibonacci sequence
 *          appearing as one of the clues in _The Da Vinci Code_.
 * https://oeis.org/A117540
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, dumb, fini
 * Formula:   a(n) = F_{n+c[(n-1)%8]}, where F_1=1, F_2=1 and c = {6, 2, 0, 4,
 *            -4, -4, -1, -3}
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static const int shift[8] = {6, 2, 0, 4, -4, -4, -1, -3};
    int s = shift[(n - 1) & 7];

    idx_t fibIndex = n;
    if (s >= 0) {
        fibIndex += (idx_t)s;
    } else {
        fibIndex -= (idx_t)(-s);
    }

    /* static storage for Fibonacci numbers */
    static seq_t *fib = NULL;
    static size_t fib_cap = 0;
    static idx_t fib_last = 0;

    /* ensure base cases */
    if (fib_last == 0) {
        size_t init_cap = 2;
        fib = (seq_t *)malloc(init_cap * sizeof(seq_t));
        fib_cap = init_cap;
        fib[0] = (seq_t)1;   /* F1 */
        fib[1] = (seq_t)1;   /* F2 */
        fib_last = 2;
    }

    /* extend array and compute missing Fibonacci values */
    if (fibIndex > fib_last) {
        if ((idx_t)fib_cap < fibIndex) {
            size_t new_cap = fib_cap ? fib_cap * 2 : 2;
            while ((idx_t)new_cap < fibIndex) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(fib, new_cap * sizeof(seq_t));
            fib = tmp;
            fib_cap = new_cap;
        }
        for (idx_t i = fib_last + 1; i <= fibIndex; ++i) {
            fib[i - 1] = fib[i - 2] + fib[i - 3];
        }
        fib_last = fibIndex;
    }

    return fib[fibIndex - 1];
}
