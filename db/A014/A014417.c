/*
 * A014417  Representation of n in base of Fibonacci numbers (the Zeckendorf
 *          representation of n). Also, binary words starting with 1 not
 *          containing 11, with the word 0 added.
 * https://oeis.org/A014417
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base, nice
 * Formula:   a(n) is the Zeckendorf representation of n written with * bits
 *            in reverse order (coefficients of Fib_2, Fib_3, Fib_4, ...)
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 143, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    /* a(0) = 0 */
    if (n == 0)
        return (seq_t)0;

    /* Fibonacci cache (sequence: F_0=0, F_1=1, F_i = F_{i-1}+F_{i-2}) */
    static seq_t *fib = NULL;
    static size_t fib_cnt = 0;   /* number of elements stored */
    static size_t fib_cap = 0;   /* capacity of fib array */

    if (fib == NULL) {          /* first allocation */
        fib_cap = 64;
        fib = alloc_seq_array(fib_cap);
        if (!fib)
            return (seq_t)0;    /* allocation failed */
        fib[0] = (seq_t)0;
        fib[1] = (seq_t)1;
        fib_cnt = 2;
    }

    /* Grow the Fibonacci array until the largest stored value exceeds n */
    while (fib_cnt >= 2 && fib[fib_cnt - 1] <= (seq_t)n) {
        /* Ensure capacity for the next value */
        if (fib_cnt == fib_cap) {
            size_t new_cap = fib_cap << 1;
            seq_t *tmp = alloc_seq_array(new_cap);
            if (!tmp)
                break; /* give up on allocation failure */
            for (size_t i = 0; i < fib_cnt; ++i)
                tmp[i] = fib[i];
            free_seq_array(fib);
            fib = tmp;
            fib_cap = new_cap;
        }
        fib[fib_cnt] = fib[fib_cnt - 1] + fib[fib_cnt - 2];
        ++fib_cnt;
    }

    /* Determine the Zeckendorf representation by a greedy algorithm */
    static unsigned char bits[200];      /* bits[i] corresponds to Fib_i */
    memset(bits, 0, sizeof(bits));
    size_t maxIdx = 0;

    seq_t remaining = (seq_t)n;
    idx_t idx = (idx_t)(fib_cnt - 1);   /* start with largest Fib <= n */

    while (idx >= 2) {
        if (fib[idx] <= remaining) {
            bits[(size_t)idx] = 1;
            if ((size_t)idx > maxIdx)
                maxIdx = (size_t)idx;
            remaining -= fib[idx];
            if (idx <= 2)
                break;
            idx -= 2;                  /* skip the next Fibonacci number */
        } else {
            if (idx <= 2)
                break;
            --idx;
        }
    }

    /* Build the decimal integer from the bits, starting with the lowest
     * index (Fib_2) and ending with the highest used index (maxIdx).
     * Since the bits were collected from Fib_2 upward, the desired
     * reverse order is obtained by processing indices in descending order. */
    seq_t result = (seq_t)0;
    for (int i = (int)maxIdx; i >= 2; --i) {
        result = result * (seq_t)10 + (seq_t)bits[i];
    }

    return result;
}
