/*
 * A181691  Numbers k such that Fibonacci(k) has exactly two 1's.
 * https://oeis.org/A181691
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 44, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   decimal_add
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the n‑th member of OEIS A181691:  the indices k such that
   Fibonacci(k) contains exactly two decimal digits '1'.
   The function works for any seq_t (uint8_t/uint16_t/.../unsigned __int128
   or arbitrary‑precision integer) by computing Fibonacci numbers
   as arbitrary‑length decimal big integers, thereby avoiding overflow
   for 64‑bit seq_t.
   The implementation caches previously found indices, so subsequent
   calls with larger or smaller n are instantaneous. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/decimal_add.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    /* static cache of found indices */
    static idx_t *found = NULL;
    static size_t found_len = 0, found_cap = 0;

    /* static buffers for Fibonacci numbers in decimal (LSD first) */
    static uint8_t *fib_a = NULL, *fib_b = NULL, *fib_c = NULL;
    static size_t len_a = 0, len_b = 0, len_c = 0;
    static size_t cap_a = 0, cap_b = 0, cap_c = 0;
    static idx_t max_k_computed = 0;  /* index of current fib_b */

    /* one‑time initialization */
    if (fib_a == NULL) {
        fib_a = (uint8_t *)malloc(1);
        fib_a[0] = 0;
        len_a = 1;
        cap_a = 1;

        fib_b = (uint8_t *)malloc(1);
        fib_b[0] = 1;
        len_b = 1;
        cap_b = 1;

        fib_c = NULL;
        cap_c = 0;
        len_c = 0;

        max_k_computed = 1;  /* fib_b holds F1 */
    }

    /* generate more terms until we have n results */
    while (found_len < n) {
        /* compute next Fibonacci number: fib_c = fib_a + fib_b */
        decimal_add(fib_a, len_a, fib_b, len_b,
                    &fib_c, &len_c, &cap_c);

        /* advance indices: fib_a <- fib_b, fib_b <- fib_c */
        uint8_t *tmp = fib_a;      uint8_t *tmpb = fib_b;
        size_t tmplen = len_a;     size_t tmplenb = len_b;
        size_t tmpcap = cap_a;     size_t tmpcapb = cap_b;

        fib_a = fib_b;   len_a = len_b;   cap_a = cap_b;
        fib_b = fib_c;   len_b = len_c;   cap_b = cap_c;

        fib_c = tmp;     len_c = tmplen;  cap_c = tmpcap;

        ++max_k_computed;   /* index of the number just computed */

        /* count '1' digits in fib_b (now the newest Fibonacci number) */
        unsigned int ones = 0;
        for (size_t i = 0; i < len_b; ++i)
            if (fib_b[i] == 1) ++ones;

        /* store the index if exactly two ones */
        if (ones == 2) {
            if (found_len == found_cap) {
                found_cap = found_cap ? found_cap * 2 : 4;
                found = (idx_t *)realloc(found,
                                         found_cap * sizeof(idx_t));
            }
            found[found_len++] = max_k_computed;
        }
    }

    /* return the requested element (1‑based index) */
    return (seq_t)found[n - 1];
}
