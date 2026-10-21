/*
 * A074487  a(1)=1, then "jump over next square": a(n) =
 *          2*(a(n-1)+1)^2-a(n-1).
 * https://oeis.org/A074487
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1) = 1, a(n) = 2 * (a(n‑1) + 1)^2 – a(n‑1) (n ≥ 2)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Each term is computed from the previous one.  No auxiliary data
 * structures or hard‑coded values are used.  The code works for
 * arbitrary‑precision seq_t (via operator overloading in C++) or
 * for any unsigned integer type. */
seq_t a(idx_t n)
{
    if (n < 1)            /* Sequence is defined only for n ≥ 1 */
        return 0;

    seq_t prev = 1;       /* a(1) */
    if (n == 1)
        return prev;

    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t t = prev + 1;           /* t = a(i‑1) + 1 */
        prev = 2 * t * t - prev;      /* a(i) = 2*t^2 - a(i‑1) */
    }

    return prev;          /* prev now holds a(n) */
}
