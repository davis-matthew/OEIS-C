/*
 * A120201  a(1)=8; a(n)=floor((71+sum(a(1) to a(n-1)))/8).
 * https://oeis.org/A120201
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 8; a(n) = floor((71 + sum_{k=1}^{n-1} a(k)) / 8) for n ≥
 *            2 (OEIS A120201) * The recurrence can be rewritten to compute
 *            a(n) from a(n‑1) and a small remainder r that stays in the range
 *            0..7, avoiding any intermediate overflow. This works with any
 *            seq_t that supports the usual integer operat...
 * Range:     u8 n <= 30, u16 n <= 77, u32 n <= 171, u64 n <= 359,
 *            u128 n <= 736, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        /* Behaviour for indices below the defined offset is irrelevant. */
        return 0;
    }

    /* a(1) is fixed */
    seq_t a_val = 8;

    if (n == 1) return a_val;

    /* r tracks (S_{k-1} + 71) mod 8 for the current k.  For k=1 we have
       S_0 = 0 ⇒ r = 71 mod 8 = 7. */
    unsigned r = 7;

    /* Iterate from k = 1 up to n‑1 to obtain a(n).  The update formula
       is derived from the definition of a(n).  It uses only 64‑bit
       arithmetic on a_val and r, so no overflow occurs in the 64‑bit
       build, while arbitrary‑precision seq_t types also support the
       same operations. */
    for (idx_t k = 1; k < n; ++k) {
        seq_t q   = a_val >> 3;          /* floor(a_val / 8)          */
        unsigned rem = (unsigned)a_val & 7U;  /* a_val mod 8            */

        /* a_{k+1} = floor((9*a_k + r) / 8)
           = 9*q + rem + floor((rem + r)/8)     (no overflow)       */
        seq_t a_next = 9 * q + rem + ((rem + r) >> 3);

        /* new remainder r' = (9*a_k + r) mod 8
           = (rem + r) mod 8                       (no overflow)   */
        unsigned r_next = (rem + r) & 7U;

        a_val = a_next;
        r     = r_next;
    }

    return a_val;
}
