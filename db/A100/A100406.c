/*
 * A100406  a(n) = repeating period of the digital roots of the sequence {m^n,
 *          m=1,2,3...}.
 * https://oeis.org/A100406
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = repeating period of the digital roots of the powers of n
 *            (digital roots of n^k for k=0,1,2,...)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* r = n mod 9  (uint64_t for safety with large n) */
    uint64_t r = n % 9ULL;

    /* Store first index seen for each residue 0..8, -1 if not seen yet */
    int first[9];
    for (int i = 0; i < 9; ++i) first[i] = -1;

    /* On each step we store the digital root corresponding to the current
       residue. State 0 corresponds to digital root 9.  */
    int dr[20];
    int idx = 0;
    int state = 1 % 9;            /* n^0 mod 9 = 1 */
    while (first[state] == -1) {
        first[state] = idx;
        dr[idx] = (state == 0) ? 9 : state;
        ++idx;
        state = (int)((1ULL * state * r) % 9ULL);
    }

    /* Cycle starts at first[state] and its length is idx - first[state] */
    int cycle_start = first[state];
    int cycle_len   = idx - cycle_start;

    seq_t result = 0;
    for (int i = cycle_start; i < cycle_start + cycle_len; ++i) {
        result = result * (seq_t)10 + (seq_t)dr[i];
    }
    return result;
}
