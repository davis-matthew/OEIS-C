/*
 * A041510  Numerators of continued fraction convergents to sqrt(272).
 * https://oeis.org/A041510
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 19, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A041510: numerators of continued‑fraction convergents to sqrt(272).
 *
 * The continued fraction of sqrt(272) is [16; 2, 32, 2, 32, …],
 * i.e. a0 = 16 and for i ≥ 1, a_i = 2 if i is odd, otherwise 32.
 *
 * Let p_{-2} = 0, p_{-1} = 1.  Then for n ≥ 0:
 *     p_n = a_n * p_{n-1} + p_{n-2}.
 *
 * The sequence a(n) requested by the OEIS entry is p_n.
 * The first few terms are:
 *     a(0)=16, a(1)=33, a(2)=1072, …
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)16;
    if (n == 1)
        return (seq_t)33;

    seq_t prev2 = (seq_t)16;   /* p_{n-2}  (starts with p_0) */
    seq_t prev1 = (seq_t)33;   /* p_{n-1}  (starts with p_1) */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t ai = (i & 1) ? (seq_t)2 : (seq_t)32;   /* period 2 */
        seq_t cur = ai * prev1 + prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
