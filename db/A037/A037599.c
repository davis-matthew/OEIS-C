/*
 * A037599  Base 6 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,3,0.
 * https://oeis.org/A037599
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = integer whose base‑6 representation is the first n digits
 *            of the repeating pattern 1,3,0. Recurrence: a(1)=1,
 *            a(n+1)=6*a(n)+d where d is the next digit (1 if position≡1 mod
 *            3, 3 if ≡2 mod 3, 0 otherwise)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = 0;      /* current value */
    unsigned rem = 0;   /* position in the 1‑3‑0 cycle: 0→1, 1→3, 2→0 */

    for (idx_t i = 1; i <= n; ++i) {
        val *= 6;                      /* shift left one base‑6 digit */

        if (rem == 0)                 /* digit 1 */
            val += 1;
        else if (rem == 1)             /* digit 3 */
            val += 3;
        /* rem == 2 ⇒ digit 0, nothing to add */

        /* advance cycle */
        rem++;
        if (rem == 3) rem = 0;
    }

    return val;
}
