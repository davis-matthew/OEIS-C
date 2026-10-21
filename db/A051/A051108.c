/*
 * A051108  Numbers described with a single word in American English.
 * https://oeis.org/A051108
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, word
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 29, u32 n <= 31, u64 n <= 34, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers that have a single-word name in American English:
   a(n) = n                    for 0 ≤ n ≤ 20
   a(n) = 20 + 10*(n-20)       for 21 ≤ n ≤ 27
   a(n) = 10^e                 for n ≥ 28
   where e = 2 if n==28,
           e = 3 if n==29,
           else e = 3*(n-28). */
#include <stdlib.h>
seq_t a(idx_t n)
{
    if (n <= 20)
        return n;
    if (n <= 27)
        return 20 + 10 * (n - 20);

    static seq_t *pow10 = NULL;
    static size_t pow10_size = 0;
    size_t e;

    if (n == 28)
        e = 2;
    else if (n == 29)
        e = 3;
    else
        e = 3 * (n - 28);

    if (pow10_size <= e) {
        size_t new_size = e + 1;
        seq_t *tmp = (seq_t*)realloc(pow10, new_size * sizeof(seq_t));
        if (!tmp)
            abort();   /* out of memory */
        pow10 = tmp;
        for (size_t i = pow10_size; i < new_size; ++i) {
            if (i == 0)
                pow10[0] = 1;
            else
                pow10[i] = pow10[i - 1] * 10;
        }
        pow10_size = new_size;
    }
    return pow10[e];
}
