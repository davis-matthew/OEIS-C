/*
 * A037634  Base 6 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 2,1,3.
 * https://oeis.org/A037634
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A037634: Base 6 digits are, in order, the first n terms of the
   periodic sequence with initial period 2,1,3.
   Recurrence:
     a(1) = 2
     a(n) = 6 * a(n-1) + d_{n-1}
   where the digit sequence d_0,d_1,d_2,… is 2,1,3,2,1,3,… repeating every 3 terms. */

seq_t a(idx_t n)
{
    seq_t val = 0;
    for (idx_t i = 1; i <= n; ++i) {
        unsigned d;
        switch ((i - 1) % 3) {
            case 0: d = 2; break;  /* position 1,4,7,… */
            case 1: d = 1; break;  /* position 2,5,8,… */
            default: d = 3; break; /* position 3,6,9,… */
        }
        val = val * 6 + d;
    }
    return val;
}
