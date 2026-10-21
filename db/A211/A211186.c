/*
 * A211186  Smallest strictly increasing sequence such that no term is the sum
 *          of any two digits of the sequence.
 * https://oeis.org/A211186
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements A211186.  The first four terms are 1,2,4,7.  For n ≥ 5
   set m = n‑5.  Write m in base‑4 with the smallest number L of digits
   such that 4^L > m.  Map each base‑4 digit d to the decimal digit
   (0→4, 1→7, 2→8, 3→9) and concatenate the digits to form the result.
   This yields the nth term of A211186. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* first four terms */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)4;
    if (n == 4) return (seq_t)7;

    /* n >= 5 */
    seq_t m = (seq_t)(n - 5);          /* offset into the 2+ digit block */
    int L = 2;                         /* minimum length is 2 digits */
    seq_t groupSize = (seq_t)16;       /* 4^2 */

    while (m >= groupSize) {           /* find appropriate length L */
        m -= groupSize;
        ++L;
        groupSize *= (seq_t)4;
    }

    /* store base‑4 digits (least significant last) */
    uint8_t digits[32];
    int pos = L - 1;
    seq_t rem = m;
    while (rem > (seq_t)0) {
        digits[pos] = (uint8_t)(rem % (seq_t)4);
        rem /= (seq_t)4;
        --pos;
    }
    while (pos >= 0) {
        digits[pos] = 0;
        --pos;
    }

    /* digit mapping: 0→4, 1→7, 2→8, 3→9 */
    static const seq_t map[4] = { (seq_t)4, (seq_t)7, (seq_t)8, (seq_t)9 };

    seq_t result = (seq_t)0;
    for (int i = 0; i < L; ++i) {
        result = result * (seq_t)10 + map[digits[i]];
    }

    return result;
}
