/*
 * A048333  Numbers that are repdigits in base 8.
 * https://oeis.org/A048333
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 36, u32 n <= 73, u64 n <= 148,
 *            u128 n <= 297, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A048333 – Numbers that are repdigits in base 8
   a(0) = 0
   For n ≥ 1:
   let k = ⌈n/7⌉                // number of digits of the repdigit
   let d = n - (k-1)·7  (1 ≤ d ≤ 7)  // digit value
   Rk = (8ᵏ – 1)/7  = 8·Rk‑1 + 1  (R1 = 1)
   a(n) = d · Rk
   Recurrence (for n>13):
   a(n) = 9·a(n-7) – 8·a(n-14)
   Initial values for n = 0…13 are 0,1,2,3,4,5,6,7,9,18,27,36,45,54
   The algorithm below computes a(n) directly from the formula
   using only integer arithmetic.  It works for any seq_t and idx_t
   type supplied by the build.  No heap allocation or external
   utilities are required. */

seq_t a(idx_t n)
{
    /* Special case for a(0) */
    if (n == (idx_t)0)
        return (seq_t)0;

    /* Determine digit count k and digit value d */
    idx_t k = (n + (idx_t)6) / (idx_t)7;           /* ceil(n/7) */
    idx_t d = n - (k - (idx_t)1) * (idx_t)7;      /* 1..7 */

    /* Compute Rk = (8^k – 1)/7 via recurrence Rk = 8·Rk‑1 + 1 */
    seq_t Rk = (seq_t)1;   /* R1 = 1 */
    for (idx_t i = (idx_t)1; i < k; ++i) {
        Rk = Rk * (seq_t)8 + (seq_t)1;
    }

    /* a(n) = d · Rk */
    return (seq_t)d * Rk;
}
