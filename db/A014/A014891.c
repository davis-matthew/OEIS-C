/*
 * A014891  n is equal to the number of 2's in all numbers <= n written in
 *          base 6.
 * https://oeis.org/A014891
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 15, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A014891:
 * a(n) is the n-th integer N such that the total number of digit 2
 * in the base‑6 representations of all integers 1..N equals N.
 */

#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t found = 0;          /* how many solutions have been found */
    idx_t   current = 1;     /* current integer being examined */
    seq_t   count_two = 0;   /* cumulative count of digit 2 up to current */

    for (;;) {
        /* Count digit 2 in the base‑6 representation of current */
        idx_t x = current;
        while (x != 0) {
            if (x % 6 == 2)
                count_two++;
            x /= 6;
        }

        /* Check if current satisfies the defining property */
        if (count_two == current) {
            found++;
            if (found == n)
                return (seq_t)current;
        }

        /* Advance to the next integer */
        current++;
    }
}
