/*
 * A221367  The simple continued fraction expansion of F(x) := Product_{n >=
 *          0} (1 - x^(4*n+3))/(1 - x^(4*n+1)) when x = (1/2)*(9 - sqrt(77)).
 * https://oeis.org/A221367
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, cofr
 * Formula:   a(n) = 1 for even n. For odd n let k = (n+1)/2. Define b0 = 2,
 *            b1 = 9 and for m ≥ 2, bm = 9·b_{m-1} – b_{m-2}. Then a(n) = bm –
 *            2. (This follows from a(2n‑1)=αⁿ+βⁿ–2 with α+β=9, αβ=1.)
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 20, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Even indices (including 0) are 1. */
    if ((n & 1ULL) == 0)
        return (seq_t)1;

    /* Odd index: compute the nth odd term via the recurrence. */
    idx_t k = (n >> 1) + 1;       /* (n+1)/2  → 1 for n=1, 2 for n=3, … */

    if (k == 1)                   /* a(1) = 9 – 2 = 7 */
        return (seq_t)7;

    seq_t b_prev2 = (seq_t)2;     /* b0  */
    seq_t b_prev1 = (seq_t)9;     /* b1  */
    seq_t b_cur   = b_prev1;      /* will hold b_k */

    for (idx_t i = 2; i <= k; ++i) {
        b_cur   = 9 * b_prev1 - b_prev2;  /* bm = 9·b_{m-1} – b_{m-2} */
        b_prev2 = b_prev1;
        b_prev1 = b_cur;
    }
    return b_cur - (seq_t)2;       /* a(2k‑1) = b_k – 2 */
}
