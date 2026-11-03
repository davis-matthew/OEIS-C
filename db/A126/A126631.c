/*
 * A126631  a(n) is the number of nonnegative integers k less than 10^n such
 *          that the decimal representation of k lacks the digit 1, at least
 *          one of digits 2,3, at least one of digits 4,5 and at least one of
 *          digits 6,7,8,9.
 * https://oeis.org/A126631
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 16*6^n - 40*5^n + 44*4^n - 26*3^n + 8*2^n - 1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Validated: standard, pass, 36 terms through n = 936 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for n >= 1 (index 1 corresponds to a(1)=9).
   Uses the closed‑form expression and a 6‑term linear recurrence for
   fast evaluation.  All arithmetic is carried out on seq_t, which may
   be an arbitrary‑precision unsigned integer type. */
static seq_t base_term(idx_t m)
{
    seq_t t2 = 1, t3 = 1, t4 = 1, t5 = 1, t6 = 1;
    for (idx_t i = 0; i < m; ++i) {
        t2 *= (seq_t)2;
        t3 *= (seq_t)3;
        t4 *= (seq_t)4;
        t5 *= (seq_t)5;
        t6 *= (seq_t)6;
    }
    seq_t left  = ((seq_t)16) * t6 + ((seq_t)44) * t4 + ((seq_t)8) * t2;
    seq_t right = ((seq_t)40) * t5 + ((seq_t)26) * t3 + (seq_t)1;
    return left - right; /* L >= R for all m >= 1 */
}

seq_t a(idx_t n)
{
    if (n < (idx_t)1) return (seq_t)0; /* undefined for n<1 */

    /* First six terms are computed directly */
    if (n <= (idx_t)6) return base_term(n);

    /* Use a 6‑term recurrence:
       a(n) = 21*a(n-1) - 175*a(n-2) + 735*a(n-3)
              - 1624*a(n-4) + 1764*a(n-5) - 720*a(n-6)   */
    seq_t buf[6];
    buf[0] = base_term((idx_t)6); /* a(6) */
    buf[1] = base_term((idx_t)5); /* a(5) */
    buf[2] = base_term((idx_t)4); /* a(4) */
    buf[3] = base_term((idx_t)3); /* a(3) */
    buf[4] = base_term((idx_t)2); /* a(2) */
    buf[5] = base_term((idx_t)1); /* a(1) */

    for (idx_t m = (idx_t)7; m <= n; ++m) {
        seq_t left  = ((seq_t)21) * buf[0] + ((seq_t)735) * buf[2] + ((seq_t)1764) * buf[4];
        seq_t right = ((seq_t)175) * buf[1] + ((seq_t)1624) * buf[3] + ((seq_t)720) * buf[5];
        seq_t next = left - right;  /* left >= right for all n>=7 */

        /* shift buffer: drop a(n-6), insert a(n) */
        buf[5] = buf[4];
        buf[4] = buf[3];
        buf[3] = buf[2];
        buf[2] = buf[1];
        buf[1] = buf[0];
        buf[0] = next;
    }

    return buf[0]; /* a(n) */
}
