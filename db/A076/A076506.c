/*
 * A076506  Expansion of x*(1+3*x+12*x^2)/(1-24*x^3).
 * https://oeis.org/A076506
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1, a(2) = 3, a(3) = 12 a(n) = 24 · a(n‑3) for n > 3
 *            (equivalently, a(n) = 24^k · a(r) with r = n mod 3, r≠0→r else
 *            3)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 21, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Ensure n ≥ 1 for the definition; extra case 0 returns 0 harmlessly */
    if (n == 0)
        return 0;

    /* Base cases for n = 1,2,3 */
    if (n == 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)3;
    if (n == 3)
        return (seq_t)12;

    /* For n > 3 compute 24^k where k = ⌊(n‑1)/3⌋ and multiply by the
       appropriate initial value according to n mod 3. */
    idx_t k = (n - 1) / 3;          /* number of full 24‑multiplications */
    seq_t pow = (seq_t)1;
    for (idx_t i = 0; i < k; ++i)
        pow *= (seq_t)24;          /* multiply by 24 k times */

    /* n % 3 == 1 → multiply by a(1)=1
       n % 3 == 2 → multiply by a(2)=3
       n % 3 == 0 → multiply by a(3)=12   */
    switch (n % 3)
    {
        case 1:  return pow;                 /* 24^k · 1   */
        case 2:  return pow * (seq_t)3;      /* 24^k · 3   */
        default: return pow * (seq_t)12;     /* 24^k · 12  */
    }
}
