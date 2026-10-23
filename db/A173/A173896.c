/*
 * A173896  Exponents in the prime factorization of 43252003274489856000, the
 *          number of possible moves for a 3 X 3 X 3 Rubik's Cube: 2^27 * 3^14
 *          * 5^3 * 7^2 * 11^1.
 * https://oeis.org/A173896
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy, changed
 * Formula:   a(n) = A124010(A075152(3), n)
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence gives the exponents in the prime factorization of
   43252003274489856000 = 2^27 · 3^14 · 5^3 · 7^2 · 11^1.
   Thus a(1)=27, a(2)=14, a(3)=3, a(4)=2, a(5)=1, and a(n)=0 for all n>5.   */

seq_t a(idx_t n)
{
    /* static lookup for the five known exponents */
    static const seq_t exponents[] = {27, 14, 3, 2, 1};

    /* Indexing is 1‑based */
    if (n == 0)          /* offset safety: ignore calls below 1 */
        return 0;
    if (n <= 5)
        return exponents[n - 1];
    return 0;            /* the number has only 5 prime factors */
}
