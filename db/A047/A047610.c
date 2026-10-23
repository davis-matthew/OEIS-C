/*
 * A047610  Positive integers that are congruent to {1, 4, 5} mod 8.
 * https://oeis.org/A047610
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-3)+8 with a(1)=1, a(2)=4, a(3)=5
 * Range:     u8 n <= 96, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence repeats the residues 1, 4, 5 modulo 8.
       For n≥1 we can compute the term directly. */
    if (n == 0) return 0;          /* undefined for n<1; return 0 as a guard */

    const seq_t base[3] = {1, 4, 5};   /* base[m] for m = (n-1)%3 */
    idx_t m = (n - 1) % 3;             /* 0,1,2 */
    idx_t k = (n - 1) / 3;             /* how many full blocks of 3 terms */

    /* a(n) = base[m] + 8 * k   (all in seq_t to avoid overflow in idx_t) */
    return base[m] + (seq_t)8 * (seq_t)k;
}
