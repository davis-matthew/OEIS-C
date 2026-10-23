/*
 * A033399  a(n) = floor(79/n).
 * https://oeis.org/A033399
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ⌊79 / n⌋
 * Range:     u8 n <= 92, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS sequence A033399: floor(79/n).
   The function accepts an unsigned 64‑bit index n (offset 1) and
   returns the result in the integer type seq_t.  Zero is returned
   for n = 0, for safety. */
seq_t a(idx_t n)
{
    /* n ≥ 1 in normal use.  Floor division of unsigned types
       automatically performs truncation toward zero.               */
    if (n == 0)
        return 0;
    return ((seq_t)79) / (seq_t)n;
}
