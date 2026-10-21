/*
 * A097922  G.f.: (1-x^4)*(1-x^10)/((1-x)*(1-x^2)^2*(1-x^3)*(1-x^5)).
 * https://oeis.org/A097922
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 + ceil((n^2 - n)/3) for n >= 2, a(0) = 1, a(1) = 1
 * Range:     u8 n <= 16, u16 n <= 312, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2)  /* a(0) and a(1) */
        return (seq_t)1;

    /* compute a(n) for n >= 2 */
    seq_t nn = (seq_t)n;          /* promote idx_t to seq_t */
    seq_t num = nn * nn - nn;     /* n^2 - n   (non‑negative for n>=2) */
    seq_t ceil_val = (num + 2) / 3; /* ceil((n^2 - n)/3) */
    return ceil_val + 2;            /* add the remaining 2 */
}
