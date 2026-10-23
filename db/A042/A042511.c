/*
 * A042511  Denominators of continued fraction convergents to sqrt(783).
 * https://oeis.org/A042511
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=a(1)=1.
   For n ≥ 2:
   - if n is even:  a(n) = 54*a(n-1) + a(n-2)
   - if n is odd:   a(n) = a(n-1) + a(n-2)   */
/* Computes the n‑th term of OEIS A042511 (offset 0). */
seq_t a(idx_t n)
{
    if (n == 0 || n == 1)
        return (seq_t)1;

    seq_t a0 = (seq_t)1;      /* a(n-2) */
    seq_t a1 = (seq_t)1;      /* a(n-1) */
    seq_t cur;

    idx_t i;
    for (i = 2; i <= n; ++i) {
        if ((i & 1) == 0) {          /* even index */
            cur = (seq_t)54 * a1 + a0;
        } else {                     /* odd index */
            cur = a1 + a0;
        }
        a0 = a1;
        a1 = cur;
    }
    return a1;
}
