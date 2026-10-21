/*
 * A085767  Smallest m such that n divides the pentagonal number A000326(m).
 * https://oeis.org/A085767
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 193, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the smallest m such that n divides the pentagonal number
   A000326(m), i.e., pe(m) = m*(3*m-1)/2.  The algorithm iterates
   over m, using the recurrence pe(m+1) = pe(m) + 3*m + 1, and keeps
   only the value modulo n to keep numbers bounded. */
#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;            /* undefined for n=0, but guard */
    idx_t m = 1;                     /* current m */
    idx_t pe_mod = 1 % n;            /* pe(1) mod n */

    if (pe_mod == 0) return (seq_t)m;

    while (1) {
        /* add the increment pe(m+1)-pe(m) = 3*m + 1 */
        idx_t diff = (3 * m + 1) % n;
        pe_mod = (pe_mod + diff) % n;
        m++;
        if (pe_mod == 0) return (seq_t)m;
    }
}
