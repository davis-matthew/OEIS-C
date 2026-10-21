/*
 * A059140  A hierarchical sequence (S(W2{2}*c) - see A059126).
 * https://oeis.org/A059140
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*(-5 + 7*2^n) - 3*n
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 28, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For small-seq_t types avoid shifting beyond its width. */
    if (sizeof(seq_t) <= 8 && n >= 64) {
        /* Fall back to the linear recurrence:
           a(n) = 4*a(n-1) - 5*a(n-2) + 2*a(n-3)   (n > 2)
           with a(0)=4, a(1)=15, a(2)=40.  This
           avoids undefined behaviour in the bit-shift
           above, and is sufficient for the tested
           values that fit in 64 bits. */
        seq_t a0 = (seq_t)4, a1 = (seq_t)15, a2 = (seq_t)40;
        if (n == 0) return a0;
        if (n == 1) return a1;
        if (n == 2) return a2;
        for (idx_t i = 3; i <= n; ++i) {
            seq_t a3 = (seq_t)4 * a2 - (seq_t)5 * a1 + (seq_t)2 * a0;
            a0 = a1; a1 = a2; a2 = a3;
        }
        return a2;
    }

    /* Compute 2^n safely for types that support it. */
    seq_t pow2 = ((seq_t)1) << n;          /* 2^n  */
    seq_t v   = ((seq_t)7) * pow2;        /* 7*2^n  */
    v -= (seq_t)5;                        /* 7*2^n - 5  */
    v <<= 1;                              /* 2*(7*2^n - 5)  */
    v -= ((seq_t)3) * (seq_t)n;           /* 2*(-5+7*2^n)-3*n  */
    return v;
}
