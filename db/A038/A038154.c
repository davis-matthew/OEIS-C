/*
 * A038154  a(n) = n! * Sum_{k=0..n-2} 1/k!.
 * https://oeis.org/A038154
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Σ_{k=0}^{n‑2} 1/k!
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence (for n ≥ 3): a(0)=0, a(1)=0, a(2)=2, a(3)=12, */
/* and a(n) = n*a(n-1) + n*(n-1)                         */
seq_t a(idx_t n)
{
    if (n < 2)              /* a(0) = a(1) = 0 */
        return 0;
    if (n == 2)
        return 2;
    if (n == 3)
        return 12;

    seq_t result = 12;      /* a(3) */
    for (idx_t i = 4; i <= n; ++i) {
        /* result = i*result + i*(i-1); */
        result = (seq_t)i * result + (seq_t)i * ((seq_t)i - 1);
    }
    return result;
}
