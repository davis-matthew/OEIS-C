/*
 * A152098  Quartic product sequence: m = 4; p = 4^3; a(n) =
 *          Product_{k=1..(n-1)/2} ( 1 + m*cos(k*Pi/n)^2 + p*cos(k*Pi/n)^4 ).
 * https://oeis.org/A152098
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Product_{k=1..(n-1)/2} ( 1 + 4*cos(k*Pi/n)^2 +
 *            64*cos(k*Pi/n)^4 )
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 19, u64 n <= 36, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=1, a(2)=1, a(3)=6, a(4)=19,
   a(n) = a(n-1) + 9*a(n-2) + 4*a(n-3) - 16*a(n-4)  (n>=5) */
seq_t a(idx_t n)
{
    const seq_t init[5] = { (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)6, (seq_t)19 };
    if (n < 5) return init[n];
    seq_t a0 = init[0];
    seq_t a1 = init[1];
    seq_t a2 = init[2];
    seq_t a3 = init[3];
    seq_t a4 = init[4];
    for (idx_t i = 5; i <= n; ++i)
    {
        seq_t next = a4 + ((seq_t)9 * a3) + ((seq_t)4 * a2) - ((seq_t)16 * a1);
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = next;
    }
    return a4;
}
