/*
 * A037524  Base 7 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 2,1,0.
 * https://oeis.org/A037524
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 7 * a(n-1) + d(n) where d(n) repeats 2,1,0 (d(1)=2,
 *            d(2)=1, d(3)=0, d(4)=2, …)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* No need to handle n==0; the sequence is defined only for n ≥ 1. */
    seq_t value = 0;                 /* will hold the accumulated result   */
    for (idx_t i = 0; i < n; ++i) {  /* i = 0 … n−1  (base‑7 digit positions) */
        unsigned d;                 /* digit to append (small integer)     */
        /* pattern 2,1,0 repeats every 3 digits */
        switch (i % 3) {
            case 0: d = 2; break;   /* positions 1,4,7,… */
            case 1: d = 1; break;   /* positions 2,5,8,… */
            default: d = 0; break;  /* positions 3,6,9,… */
        }
        value = value * (seq_t)7 + (seq_t)d;  /* shift left 1 base‑7 digit and add d */
    }
    return value;
}
