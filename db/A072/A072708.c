/*
 * A072708  Last digit of F(n) is 6 where F(n) is the n-th Fibonacci number.
 * https://oeis.org/A072708
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A072708: indices n such that the last digit of the nth Fibonacci number is 6.
   Formula: a(n) = 60*⌊(n‑1)/4⌋ + {21, 39, 42, 48} according to n mod 4
   (n mod 4 = 1 → 21, 2 → 39, 3 → 42, 0 → 48). */

seq_t a(idx_t n)
{
    /*  base values for residues n mod 4 (0→48, 1→21, 2→39, 3→42) */
    static const seq_t base[4] = {(seq_t)48, (seq_t)21, (seq_t)39, (seq_t)42};

    /* Number of complete blocks of 4 indices preceding n. */
    idx_t q = (n - 1) / 4;
    /* Current residue n mod 4. */
    idx_t r = n % 4;

    /* Compute result.  All intermediate values are in seq_t. */
    return base[r] + (seq_t)60 * (seq_t)q;
}
