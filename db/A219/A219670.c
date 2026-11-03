/*
 * A219670  Number of n-step paths on cubic lattice from (0,0,0) to (1,0,0)
 *          with moves in any direction on {-1,0,1}^3 and zero moves allowed.
 * https://oeis.org/A219670
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = C1(n) * C0(n)^2 where C0(n) = coefficient of t^n in (1 +
 *            t + t^2)^n = sum_{c=0..⌊n/2⌋} n! / (c! * c! * (n-2c)!) C1(n) =
 *            coefficient of t^{n+1} in (1 + t + t^2)^n = sum_{k=0..⌊(n-1)/2⌋}
 *            n! / (k! * (k+1)! * (n-2k-1)!)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 29,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 236 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t c1_term(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t term = (seq_t)n;     /* term for k = 0   */
    seq_t sum = term;
    idx_t limit = (n - 1) / 2; /* number of additional terms */
    for (idx_t k = 0; k < limit; ++k)         /* k runs up to limit‑1 */
    {
        seq_t num1 = (seq_t)(n - 2 * k - 1);   /* n‑2k‑1  */
        seq_t num2 = (seq_t)(n - 2 * k - 2);   /* n‑2k‑2  */
        term = term * num1;
        term = term * num2;
        seq_t d1 = (seq_t)(k + 1);             /* (k+1)   */
        seq_t d2 = (seq_t)(k + 2);             /* (k+2)   */
        term = term / d1;
        term = term / d2;
        sum = sum + term;
    }
    return sum;
}

static seq_t central_trinomial(idx_t n)
{
    if (n == 0)
        return 1;
    seq_t term = 1;          /* term for c = 0   */
    seq_t total = term;
    for (idx_t c = 0; c < n / 2; ++c)       /* c runs up to ⌊n/2⌋‑1   */
    {
        seq_t f1 = (seq_t)(n - 2 * c);      /* n‑2c   */
        term = term * f1;
        term = term * (f1 - 1);            /* n‑2c‑1 */
        seq_t d = (seq_t)(c + 1);          /* (c+1)² */
        term = term / d;
        term = term / d;
        total = total + term;
    }
    return total;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t c0 = central_trinomial(n);
    seq_t c1 = c1_term(n);
    seq_t res = c1 * c0 * c0;
    return res;
}
