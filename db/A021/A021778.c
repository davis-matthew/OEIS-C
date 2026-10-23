/*
 * A021778  Decimal expansion of 1/774.
 * https://oeis.org/A021778
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   decimal expansion of 1/774. a(0) = 0 (integer part). For n >= 1,
 *            a(n) is the (n+1)th digit obtained by long division of 1 by 774.
 *            The expansion has one non‑repeating digit (0) followed by a
 *            repeating cycle of length 21:
 *            0,1,2,9,1,9,8,9,6,6,4,0,8,2,6,8,7,3,3,8,5
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Requires the standard headers for boolean and size_t. */
#include <stddef.h>
#include <stdbool.h>

/* The function defined for the sequence. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* integer part */

    /* Precompute the repeating cycle only once. */
    static seq_t cycle[256];      /* enough for any small cycle */
    static size_t cycle_len = 0;  /* length of the cycle */

    if (cycle_len == 0) {
        /* The remainder after the first decimal digit is 10. */
        idx_t remainder = 10 % 774;   /* 10 */
        do {
            idx_t product = remainder * 10;          /* before modulo */
            seq_t digit = (seq_t)(product / 774);    /* quotient digit */
            cycle[cycle_len++] = digit;
            remainder = product % 774;              /* new remainder */
        } while (remainder != 10);  /* repeat until the first remainder comes back */
    }

    /* a(n) corresponds to the (n-1)th element of the cycle. */
    return cycle[(n - 1) % cycle_len];
}
