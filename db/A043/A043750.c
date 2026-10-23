/*
 * A043750  Numbers n such that number of runs in the base 2 representation of
 *          n is congruent to 4 mod 8.
 * https://oeis.org/A043750
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 52, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A043750: Numbers n such that the number of runs in the binary representation
    of n is congruent to 4 modulo 8.  This function returns the n‑th such
    integer (with a(1)=10).  It simply scans natural numbers, counts
    runs in binary, and collects those that satisfy the congruence. */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;            /* undefined for n=0 – return 0 as a safety value */

    idx_t found = 0;         /* how many terms we have seen so far */
    idx_t candidate = 1;     /* current integer to test */

    while (true) {
        /* Count runs in the binary representation of candidate */
        idx_t runs = 0;
        idx_t x = candidate;
        while (x) {
            ++runs;
            idx_t bit = x & 1;
            /* Skip over the current run of identical bits */
            while (x && ((x & 1) == bit))
                x >>= 1;
        }

        if ((runs & 7u) == 4u) {          /* runs % 8 == 4 ? */
            ++found;
            if (found == n)
                return (seq_t)candidate;  /* cast to seq_t is safe by requirement */
        }

        ++candidate;                      /* test next integer */
    }
}
