/*
 * A181270  Number of 2 X n binary matrices M with rows in strictly increasing
 *          order and rows of M*Mtranspose (mod 2) in strictly increasing
 *          order.
 * https://oeis.org/A181270
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A181270
    Formula:  a(1)=1, a(2)=2, a(3)=11, a(4)=44;
              a(n)=4*a(n-1)+4*a(n-2)-16*a(n-3)   for n>4
*/
seq_t a(idx_t n)
{
    /* safety for out‑of‑range index */
    if (n == 0) return (seq_t)0;

    switch (n) {
    case 1:  return (seq_t)1;
    case 2:  return (seq_t)2;
    case 3:  return (seq_t)11;
    case 4:  return (seq_t)44;
    default:
        /* three previous values: a(n-3), a(n-2), a(n-1) */
        seq_t a_nm3 = (seq_t)2;   /* a(2)   */
        seq_t a_nm2 = (seq_t)11;  /* a(3)   */
        seq_t a_nm1 = (seq_t)44;  /* a(4)   */

        for (idx_t i = 5; i <= n; ++i) {
            seq_t a_n = (seq_t)4 * a_nm1 + (seq_t)4 * a_nm2 - (seq_t)16 * a_nm3;
            a_nm3 = a_nm2;
            a_nm2 = a_nm1;
            a_nm1 = a_n;
        }
        return a_nm1;
    }
}
