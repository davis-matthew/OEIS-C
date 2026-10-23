/*
 * A141259  a(n) == {0,1,3,4,5,7,9,11} mod 12; n>0.
 * https://oeis.org/A141259
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 170, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A141259: numbers not congruent to 2,6,8,10 (mod 12).
   For n ≥ 1, a(n) is the n‑th positive integer that is
   ≡ 0,1,3,4,5,7,9,11 (mod 12) in increasing order.
   The nine excluded residues ∈{2,6,8,10} form 1/3 of all integers,
   so the density of the sequence is 2/3 and a(n) ~ (3/2)n. */

seq_t a(idx_t n)
{
    /* The 8 admissible residues modulo 12, sorted in ascending order.
       The first term corresponds to residue 1 (since a(1)=1). */
    static const unsigned offset[8] = { 1, 3, 4, 5, 7, 9, 11, 12 };

    /* Count how many full cycles of 8 terms are passed before the n‑th term.
       Each cycle increases the value by 12. */
    idx_t q = (n - 1) / 8;          /* number of full cycles */
    unsigned r = (unsigned)((n - 1) % 8);  /* position within current cycle */

    /* Compute the term using the formula
          a(n) = 12 * q + offset[r]   (all arithmetic in seq_t)
       Casting q to seq_t before multiplication keeps the result in the
       proper type even for arbitrary precision builds. */
    return ((seq_t)q) * 12 + offset[r];
}
