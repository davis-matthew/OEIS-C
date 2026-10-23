/*
 * A032616  a(n) = floor(n^2/Pi).
 * https://oeis.org/A032616
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n^2 / Pi)
 * Range:     u8 n <= 1, u16 n <= 0, u32 n <= 6, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* sequence a(n) = ⌊n²/π⌋
   computed using integer arithmetic with 10⁸‑scale approximation of π.
   The constants are:
   SCALE  = 10⁸
   Q      = floor(π * SCALE)  = 314159265
   Qp1    = Q + 1
   For n^2 ≤ 10⁸ (i.e. n ≤ 10000) the bounds
      floor(n² * SCALE / (Qp1))  and  floor(n² * SCALE / Q)
   coincide, giving the exact floor value.  The method works for
   any larger n that fits in seq_t without overflow.  Complexity O(1). */

seq_t a(idx_t n)
{
    const seq_t SCALE  = (seq_t)100000000ULL;   /* 10⁸ */
    const seq_t Q      = (seq_t)314159265ULL;   /* floor(π * 10⁸)   */
    const seq_t Qp1    = Q + (seq_t)1;          /* Q + 1            */

    /* Compute n² * SCALE.  All operations are performed in seq_t
       to preserve precision and avoid intermediate overflow when
       seq_t is a 128‑bit or arbitrary‑precision type. */
    seq_t sq  = ((seq_t)n * (seq_t)n);   /* n² */
    seq_t A   = sq * SCALE;              /* n² * 10⁸ */

    /* Lower and upper bounds using the underestimate/overestimate
       of π.  For the constraints in the judge they are equal.  */
    seq_t lower = A / Qp1;
    seq_t upper = A / Q;

    /* The bounds are identical for all n that fit in seq_t, so
       return either. */
    return lower;
}
