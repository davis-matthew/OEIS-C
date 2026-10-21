/*
 * A082206  Digit sum of A082205(n).
 * https://oeis.org/A082206
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, less
 * Formula:   a(n) = 3*n-2 for n <= 4; a(n) = a(n-3) + 7 for n > 4
 * Range:     u8 n <= 109, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 4)
    {
        /* a(1)=1, a(2)=4, a(3)=7, a(4)=10 */
        return (seq_t)3 * n - 2;
    }

    /* For n > 4 the recurrence a(n) = a(n-3) + 7 applies.
       Use the residue of n modulo 3 to compute the value directly. */
    switch (n % 3)
    {
        case 1: /* n ≡ 1 (mod 3), n ≥ 4 */
            return (seq_t)10 + (seq_t)7 * ((n - 4) / 3);
        case 2: /* n ≡ 2 (mod 3) */
            return (seq_t)4 + (seq_t)7 * ((n - 2) / 3);
        case 0: /* n ≡ 0 (mod 3) */
            return (seq_t)7 + (seq_t)7 * ((n - 3) / 3);
    }

    /* Unreachable due to switch covering all possible residues */
    return 0;
}
