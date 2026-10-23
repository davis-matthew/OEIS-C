/*
 * A140426  Number of multi-symmetric Steinhaus matrices of size n.
 * https://oeis.org/A140426
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^ceil(n/6) for n even, 2^ceil((n-3)/6) for n odd
 * Range:     u8 n <= 43, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute 2^exp using multiplication to avoid undefined behaviour with
   bit shifting when exp >= width of the underlying integer type.
   This works for any seq_t, including arbitrary‑precision implementations. */
static seq_t pow2_fast(idx_t exp)
{
    seq_t r = 1;
    for (idx_t i = 0; i < exp; ++i)
        r *= 2;   /* unsigned arithmetic wraps modulo 2^N, defined behaviour */
    return r;
}

/* External function that returns the n-th term of A140426. */
seq_t a(idx_t n)
{
    idx_t exp;
    if ((n & 1ULL) == 0)          /* n is even */
        exp = (n + 5) / 6;        /* ceil(n/6) */
    else                          /* n is odd */
        exp = (n + 2) / 6;        /* ceil((n-3)/6) = (n+2)/6 */
    return pow2_fast(exp);
}
