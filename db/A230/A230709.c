/*
 * A230709  Union of even odious (cf. A128309) and evil numbers (cf. A001969).
 * https://oeis.org/A230709
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 192, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) is the n-th non‑negative integer that is either even
   or has an even number of 1’s in its binary expansion (the union of
   even odious numbers and evil numbers). No simple closed form – the
   value is obtained by scanning from 0 upwards and counting qualifying
   numbers. */

/* The function is external and uses only the allowed integer
   arithmetic and bitwise operators. */

#include <stdbool.h>

/* Return true if x has an even binary population count.  */
static bool popcount_even(seq_t x)
{
    int parity = 0;            /* 0 = even, 1 = odd */
    while (x != 0) {
        x &= (x - 1);          /* clear lowest set bit */
        parity ^= 1;
    }
    return parity == 0;
}

/* Return the n-th term of the sequence (1‑based index).  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n=0, return 0 for safety */
    idx_t count = 0;               /* how many qualifying numbers seen */
    seq_t cur = 0;                 /* current integer being tested */

    while (1) {
        /* Include cur if it is even or, if odd, has an even popcount. */
        if ((cur & 1) == 0 || popcount_even(cur)) {
            ++count;
            if (count == n) return cur;
        }
        cur++;                     /* advance to next integer */
    }
}
