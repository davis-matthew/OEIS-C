/*
 * A065280  The bisection of odd terms (the <= 0 half of Z) of A065279.
 * https://oeis.org/A065280
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = a(2) = a(3) = 1. For n ≥ 4 let k be the unique integer
 *            with 1 + 3·2^k ≤ n < 1 + 3·2^(k+1). Then let pow2k = 2^k. If n -
 *            (1 + 3·pow2k) < pow2k → a(n) = 2·n – 2 (the “A‑block”) else a(n)
 *            = 2^(k+1) (the “B‑block”) (The sequence consists of alternating
 *            blocks of lengths 2^k and 2^(k+1).)
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 3)               /* a(1)=a(2)=a(3)=1 */
    {
        return (seq_t)1;
    }

    /* find k such that 1+3*2^k ≤ n < 1+3*2^(k+1) */
    idx_t limit = (n - 1) / 3;          /* limit = (n-1)/3  */
    idx_t k = 0;
    while ((k + 1) < 64)               /* safeguard against overflow */
    {
        idx_t next = ((idx_t)1 << (k + 1));
        if (next > limit) break;
        k++;
    }

    idx_t pow2k = ((idx_t)1 << k);      /* 2^k   */
    idx_t start = 1 + 3 * pow2k;        /* 1 + 3·2^k  */
    idx_t offset = n - start;           /* position inside the cycle */

    if (offset < pow2k)                 /* A‑block: deficit 0 */
    {
        return (seq_t) n * 2 - 2;       /* 2·n – 2   */
    }
    else                               /* B‑block: constant in the block */
    {
        return (seq_t)(pow2k << 1);    /* 2^(k+1)   */
    }
}
