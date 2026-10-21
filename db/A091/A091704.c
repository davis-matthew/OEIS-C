/*
 * A091704  Number of Barker codes (or Barker sequences) of length n up to
 *          reversals and negations.
 * https://oeis.org/A091704
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 if n=2 or n=4; 1 if n∈{3,5,7,11,13}; else 0. / (These
 *            are the only known Barker sequence lengths up to reversal and
 *            negation.)
 * Range:     u8 n <= 103, u16 n <= 103, u32 n <= 103, u64 n <= 103,
 *            u128 n <= 103, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>   /* for bool, if desired */

seq_t a(idx_t n) {
    /* Known Barker sequence lengths */
    switch (n) {
        case 2:  return (seq_t)2;
        case 3:  return (seq_t)1;
        case 4:  return (seq_t)2;
        case 5:  return (seq_t)1;
        case 7:  return (seq_t)1;
        case 11: return (seq_t)1;
        case 13: return (seq_t)1;
        default: return (seq_t)0;
    }
}
