/*
 * A130165  a(1)=1; a(n)=prime(mod(a(n-1),10)).
 * https://oeis.org/A130165
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less, easy
 * Formula:   a(1) = 1; a(n) = prime(mod(a(n-1),10))
 * Range:     u8 n <= 94, u16 n <= 94, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Return the n‑th term of A130165 */
seq_t a(idx_t n)
{
    /* Explicit initial values */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)3;
    if (n == 4) return (seq_t)5;
    if (n == 5) return (seq_t)11;

    /* For n ≥ 6 the sequence repeats 2,3,5,11 with period 4 */
    static const seq_t cycle[4] = {2, 3, 5, 11};
    idx_t offset = (n - 6) % 4;
    return cycle[offset];
}
