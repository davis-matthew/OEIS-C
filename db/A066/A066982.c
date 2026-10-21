/*
 * A066982  a(n) = Lucas(n+1) - (n+1).
 * https://oeis.org/A066982
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Lucas(n+1) - (n+1) (Lucas sequence: L0 = 2, L1 = 1, Lk =
 *            Lk-1 + Lk-2)
 * Range:     u8 n <= 10, u16 n <= 22, u32 n <= 45, u64 n <= 91, u128 n <= 183,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Offset: 1 (so a(1) is the first listed term) */
seq_t a(idx_t n)
{
    /* Lucas numbers: start with L0 = 2, L1 = 1 */
    seq_t L0 = (seq_t)2;
    seq_t L1 = (seq_t)1;

    /* Compute Lucas(n+1) by iterating the recurrence */
    idx_t target = n + (idx_t)1;     /* n >= 1, so target >= 2 */

    seq_t Ln = 0;
    for (idx_t i = 2; i <= target; ++i) {
        Ln = L0 + L1;
        L0 = L1;
        L1 = Ln;
    }
    /* Now L1 holds Lucas(target) = Lucas(n+1) */

    /* Subtract (n+1) to obtain a(n) */
    seq_t nn = (seq_t)n + (seq_t)1;
    return L1 - nn;
}
