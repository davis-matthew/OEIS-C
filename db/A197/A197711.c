/*
 * A197711  Floor((n+1/n)^10).
 * https://oeis.org/A197711
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((n + 1/n)^10)
 * Range:     u8 none, u16 n <= 2, u32 n <= 8, u64 n <= 84, u128 n <= 7131,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent integer formula:
   Let nₖ = n^k.
   a(n) = Σ_{k=5}^{10} C(10,k) * n^(2k-10)   +   floor( Σ_{k=0}^{4} C(10,k) * n^(2k) / n^10 )
   This avoids overflow for all n that fit in the 64‑bit seq_t type. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset is 1, but guard against 0 */
    seq_t nn   = (seq_t)n;
    seq_t n2   = nn * nn;               /* n² */
    seq_t n4   = n2 * n2;               /* n⁴ */
    seq_t n6   = n4 * n2;               /* n⁶ */
    seq_t n8   = n4 * n4;               /* n⁸ */
    seq_t n10  = n8 * n2;               /* n¹⁰ */

    /* Integer part (k ≥ 5) */
    seq_t big_part = n10
                    + (seq_t)10 * n8
                    + (seq_t)45 * n6
                    + (seq_t)120 * n4
                    + (seq_t)210 * n2
                    + (seq_t)252;

    /* Fractional part that may contribute an extra integer when divided by n¹⁰ */
    seq_t small_sum = (seq_t)1
                    + (seq_t)10 * n2
                    + (seq_t)45 * n4
                    + (seq_t)120 * n6
                    + (seq_t)210 * n8;

    seq_t add = small_sum / n10;  /* floor of the fractional contribution */
    return big_part + add;
}
