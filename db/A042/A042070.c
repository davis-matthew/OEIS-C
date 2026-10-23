/*
 * A042070  Numerators of continued fraction convergents to sqrt(559).
 * https://oeis.org/A042070
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   a(0)=23, a(1)=24, a(n)=c_{n-1}*a(n-1)+a(n-2) for n≥2, where the
 *            coefficient sequence c is periodic of length 20:
 *            [1,1,1,4,15,1,1,4,1,2,1,4,1,1,15,4,1,1,1,46]. This produces the
 *            numerators of the continued fraction convergents to sqrt(559)
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)23;
    if (n == 1) return (seq_t)24;

    static const seq_t period[20] = {
        (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)4, (seq_t)15,
        (seq_t)1, (seq_t)1, (seq_t)4, (seq_t)1, (seq_t)2,
        (seq_t)1, (seq_t)4, (seq_t)1, (seq_t)1, (seq_t)15,
        (seq_t)4, (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)46
    };

    seq_t a0 = (seq_t)23;   /* a(0) */
    seq_t a1 = (seq_t)24;   /* a(1) */
    idx_t i;
    for (i = 2; i <= n; ++i) {
        const seq_t coeff = period[(i - 1) % 20];
        seq_t next = a1 * coeff + a0;
        a0 = a1;
        a1 = next;
    }
    return a1;
}
