/*
 * A065187  "Greedy Dragons" permutation of the natural numbers, inverse of
 *          A065186.
 * https://oeis.org/A065187
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(q) + 5*⌊(n-1)/5⌋, where q = ((n-1) mod 5) + 1 Initial
 *            values: a(1)=1, a(2)=4, a(3)=2, a(4)=5, a(5)=3 (i.e. a(1..5) =
 *            {1,4,2,5,3})
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* not defined, safe default */
    static const seq_t base[5] = {1, 4, 2, 5, 3};
    idx_t q = (n - 1) / 5;                  /* number of complete 5‑blocks */
    unsigned int r = (unsigned int)((n - 1) % 5); /* 0‑based remainder */
    return base[r] + ((seq_t)q * 5);
}
