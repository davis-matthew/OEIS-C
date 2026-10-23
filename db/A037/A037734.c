/*
 * A037734  Base 8 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 2,1,0,3.
 * https://oeis.org/A037734
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=2; a(n)=8*a(n-1)+digit[(n-1)%4] where digit[0]=2,digit[1]=1,digit[2]=0,digit[3]=3. */

seq_t a(idx_t n)
{
    /* pattern of digits in base 8 */
    static const seq_t pat[4] = { (seq_t)2, (seq_t)1, (seq_t)0, (seq_t)3 };

    seq_t cur = 0;
    for (idx_t i = 0; i < n; ++i) {
        cur *= (seq_t)8;        /* shift left by one base‑8 digit */
        cur += pat[i & 3];      /* add next digit of the cycle 2,1,0,3 */
    }
    return cur;
}
