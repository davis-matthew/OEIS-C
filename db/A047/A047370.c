/*
 * A047370  Numbers that are congruent to {2, 3, 5} mod 7.
 * https://oeis.org/A047370
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*floor((n-1)/3) + 2^((n-1) mod 3) + 1
 * Range:     u8 n <= 110, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1)=2, a(2)=3, a(3)=5, a(4)=9, ... */

/* The function computes the OEIS sequence A047370 (numbers ≡{2,3,5} mod 7). */
seq_t a(idx_t n)
{
    /* n ≥ 1 is guaranteed by the contract */
    idx_t k = (n - 1) / 3;          /* floor((n-1)/3)                     */
    idx_t r = (n - 1) % 3;          /* (n-1) mod 3 (0,1,2)                 */

    /* 2^r + 1 gives the offsets 2, 3, 5 for r = 0,1,2 respectively. */
    seq_t offset = ((seq_t)1 << r) + (seq_t)1;

    /* 7*k might overflow idx_t but we cast to seq_t before multiplication. */
    seq_t term = ((seq_t)7 * (seq_t)k) + offset;

    return term;
}
