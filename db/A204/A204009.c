/*
 * A204009  a(n) is a binary vector for selecting distinct terms from A000124
 *          that when summed give n; it uses the greedy algorithm.
 * https://oeis.org/A204009
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses greedy choice of centered triangular numbers (A000124) to
 * express n as a sum, then returns the binary selection vector
 * encoded as decimal digits (e.g. 10011). */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define MAX_TERMS 256   /* enough for n up to about 10 000 */

seq_t a(idx_t n)
{
    /* trivial case */
    if (n == 0)
        return (seq_t)0;

    /* generate the centered triangular numbers up to n */
    seq_t terms[MAX_TERMS];
    unsigned char bits[MAX_TERMS];
    size_t len = 0;
    seq_t term = (seq_t)1;   /* first term of A000124 */
    idx_t k = 1;             /* index for the recurrence term += k */

    while (term <= (seq_t)n && len < MAX_TERMS) {
        terms[len]   = term;
        bits[len]    = 0;    /* initialise bit to 0 */
        ++len;
        term = term + (seq_t)k;
        ++k;
    }

    /* greedy selection from largest term downwards */
    seq_t rem = (seq_t)n;
    for (size_t i = len; i-- > 0; ) {
        if (rem >= terms[i]) {
            rem -= terms[i];
            bits[i] = 1;
        }
    }

    /* construct the decimal representation of the bit vector */
    seq_t result = (seq_t)0;
    for (size_t i = len; i-- > 0; ) {
        result = result * 10 + bits[i];
    }

    return result;
}
