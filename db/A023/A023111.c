/*
 * A023111  Squares that remain square when the digit 1 is appended.
 * https://oeis.org/A023111
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 7, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A023111:
   Squares that remain square when the digit 1 is appended.
   Recurrence: a(1)=0, a(2)=36, a(3)=51984,
                a(n)=1443*a(n‑1) – 1443*a(n‑2) + a(n‑3)   for n≥4 */
   
seq_t a(idx_t n)
{
    /* Handle the base values explicitly. */
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)36;
    if (n == 3) return (seq_t)51984;

    /* Use three rolling variables to keep memory constant. */
    seq_t a1 = (seq_t)0;     /* a(n‑3)  */
    seq_t a2 = (seq_t)36;    /* a(n‑2)  */
    seq_t a3 = (seq_t)51984; /* a(n‑1)  */

    /* Compute forward until we reach the desired index. */
    for (idx_t i = 4; i <= n; ++i) {
        seq_t an = (seq_t)1443 * a3 - (seq_t)1443 * a2 + a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return a3;
}
