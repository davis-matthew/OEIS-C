/*
 * A047239  Numbers that are congruent to {1, 2} (mod 6).
 * https://oeis.org/A047239
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*(n-1) - (-1)^n
 * Range:     u8 n <= 51, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A047239: numbers congruent to {1,2} (mod 6) */
seq_t a(idx_t n)
{
    /* Trivial closed‑form computation */
    seq_t N = (seq_t)n;          /* convert index to seq_t for safety */
    seq_t val = 3 * (N - (seq_t)1);   /* 3*(n-1) */

    /* (-1)^n is 1 for even n, -1 for odd n.  Subtracting it gives
       - for even n: val - 1
       - for odd n:  val + 1 (since val - (-1) = val + 1)   */
    if ((n & (idx_t)1) == 0)          /* even n */
        val -= (seq_t)1;
    else
        val += (seq_t)1;

    return val;
}
