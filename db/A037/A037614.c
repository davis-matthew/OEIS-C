/*
 * A037614  Base 7 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,3,2.
 * https://oeis.org/A037614
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the base-7 number whose first n digits * form the
 *            periodic sequence 1,3,2,1,3,2..., i.e. * a(n) = Σ_{k=1}^n d_k *
 *            7^{n-k} with d_k = 1 if k≡1 (mod 3), * d_k = 3 if k≡2 (mod 3),
 *            d_k = 2 if k≡0 (mod 3)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* iterative base‑7 construction:  result = result*7 + current_digit */
    seq_t result = 0;
    int state = 0;                      /* 0 → 1, 1 → 3, 2 → 2 */
    for (idx_t i = 0; i < n; ++i) {
        seq_t digit = (state == 0) ? 1
                         : (state == 1) ? 3
                         : 2;
        result = result * 7 + digit;
        state++;
        if (state == 3) state = 0;
    }
    return result;
}
