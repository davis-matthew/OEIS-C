/*
 * A145052  One-third of the number of n X n nonnegative integer arrays with
 *          every 3 X 3 subblock summing to 1.
 * https://oeis.org/A145052
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*3^(⌊n/3⌋) - 3 + 4*(n mod 3)*3^(⌊n/3⌋)/3 (n ≥ 3)
 * Range:     u8 n <= 12, u16 n <= 26, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* For n < 3 the behavior is irrelevant, but we guard it. */
    if (n < 3)
        return (seq_t)0;

    idx_t k = n / 3;           /* floor(n/3) */
    int r = n % 3;             /* n mod 3 */

    /* Compute 3^k.  k is at most n/3, and n≤10000 in the tests, so a few thousand
       multiplications are inexpensive even for arbitrary‑precision seq_t. */
    seq_t pow = (seq_t)1;
    for (idx_t i = 0; i < k; ++i)
        pow *= (seq_t)3;

    seq_t pow_div3 = pow / (seq_t)3;         /* integer division: 3^k is divisible by 3 */
    seq_t term = (seq_t)(4 * r) * pow_div3;  /* handles r = 0, 1, 2 */
    seq_t result = (seq_t)2 * pow - (seq_t)3 + term;

    return result;
}
