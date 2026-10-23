/*
 * A118295  Start with 20 and repeatedly reverse the digits and add 1 to get
 *          the next term.
 * https://oeis.org/A118295
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=20, a(n)=reverse_decimal(a(n-1))+1 for n>1 */
/* The sequence cycles with period 9 after the first term. */

#include <stddef.h> /* for size_t */

seq_t a(idx_t n) {
    if (n == 1)
        return (seq_t)20;

    /* cycle of values after the first term */
    static const seq_t cycle[9] = {
        (seq_t)3, (seq_t)4, (seq_t)5,
        (seq_t)6, (seq_t)7, (seq_t)8,
        (seq_t)9, (seq_t)10, (seq_t)2
    };

    /* compute index into cycle: (n-2) mod 9 */
    idx_t idx = (n - 2) % 9;
    return cycle[(size_t)idx];
}
