/*
 * A120436  Complete list of numbers that can be represented both as a product
 *          of 2 consecutive integers and as a product of 3 consecutive
 *          integers.
 * https://oeis.org/A120436
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, bref
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 3, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A120436: numbers that are both a product of two consecutive integers
   and a product of three consecutive integers.
   Known integer solutions are: 0, 6, 210. The sequence is finite,
   so for indices beyond 3 the value is defined as 0. */
seq_t a(idx_t n)
{
    switch (n) {
        case 1: return (seq_t)0;
        case 2: return (seq_t)6;
        case 3: return (seq_t)210;
        default: return (seq_t)0;
    }
}
