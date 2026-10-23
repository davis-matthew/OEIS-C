/*
 * A195062  Period 7: repeat [1, 0, 1, 0, 1, 0, 1].
 * https://oeis.org/A195062
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1 + (-1)^((n-1) mod 7)) / 2
 * Range:     u8 n <= 126, u16 n <= 126, u32 n <= 126, u64 n <= 126,
 *            u128 n <= 126, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term (offset 1) of OEIS A195062.
   The sequence repeats [1,0,1,0,1,0,1] with period 7. */
seq_t a(idx_t n)
{
    /* Since n >= 1, n-1 is safe for unsigned arithmetic. */
    idx_t r_mod7 = (n - 1) % 7;          /* 0..6 */
    idx_t parity = r_mod7 % 2;           /* 0 for even positions, 1 for odd */
    return (seq_t)(1 - parity);          /* 1 if parity==0, else 0 */
}
