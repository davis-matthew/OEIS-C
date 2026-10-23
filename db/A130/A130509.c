/*
 * A130509  a(1)=3, a(2)=1, a(3)=2, a(n+3) = 3 + a(n), for all positive
 *          integers n.
 * https://oeis.org/A130509
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-3) + 3, with a(1)=3, a(2)=1, a(3)=2
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to seq_t for arithmetic */
    seq_t n_seq = (seq_t)n;

    /* Remainder of n modulo 3 (0,1,2) */
    idx_t r = n % 3;

    /* Base values for each residue class: r=0 -> 2, r=1 -> 3, r=2 -> 1 */
    static const seq_t base[3] = { 2, 3, 1 };

    /* Compute floor division by 3 */
    seq_t div3 = n_seq / 3;

    /* Offset = floor((n - class)/3) */
    seq_t offset = div3;
    if (r == 0)             /* class 3 (r==0) needs one less offset */
        offset -= 1;

    return base[r] + 3 * offset;
}
