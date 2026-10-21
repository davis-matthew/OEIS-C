/*
 * A072835  Exponents occurring in expansion of F_9(q^2).
 * https://oeis.org/A072835
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 56, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0;  a(1)=8; a(2)=14; a(3)=18; a(4)=20;
   a(n)=a(n-4)+18 for n>0 (n ≥ 1) */
seq_t a(idx_t n)
{
    if (n == 0) /* base case */
        return (seq_t)0;

    /* Base values for residues 1..4 */
    static const seq_t base[5] = {0, 8, 14, 18, 20};

    idx_t r = n % 4;
    if (r == 0) r = 4;          /* use residue 4 for n≡0 (mod 4) */

    seq_t result = base[r];          /* the value for this residue class   */
    idx_t t = (n - r) / 4;           /* number of 4‑step increments        */
    seq_t add = (seq_t)t * (seq_t)18; /* contribution from the recurrence   */

    return result + add;
}
