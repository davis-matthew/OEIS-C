/*
 * A221953  a(n) = 5^(n-1) * n! * Catalan(n-1).
 * https://oeis.org/A221953
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*(2*n-3)*a(n-1), a(1)=1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Offset: 1 (a(1) is the first term) */

#include <stdint.h>  /* for basic integer types, if needed */

/*
 * Compute the nth term of sequence A221953.
 * The sequence satisfies the recurrence:
 *     a(1) = 1
 *     a(n) = 10 * (2*n - 3) * a(n-1)   for n >= 2
 *
 * Args:
 *   n : index (1-based)
 *
 * Returns:
 *   the value a(n) as a seq_t.  The function is safe for any n >= 1.
 */
seq_t a(idx_t n)
{
    if (n <= 1) {
        return (seq_t)1;
    }

    seq_t result = (seq_t)1;
    for (idx_t i = 2; i <= n; ++i) {
        seq_t factor = (seq_t)10 * (2 * i - 3);
        result *= factor;
    }
    return result;
}
