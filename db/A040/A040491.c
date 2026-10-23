/*
 * A040491  Continued fraction for sqrt(514).
 * https://oeis.org/A040491
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less, mult
 * Formula:   not stated
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  
    Periodic continued‑fraction partial quotients of sqrt(514).  
    a(0) = 22, and for n ≥ 1  
    a(n) = [1, 2, 22, 2, 1, 44][(n‑1) mod 6].
*/
seq_t a(idx_t n)
{
    if (n == 0) return 22;

    /* Periodic part of the continued fraction for √514 */
    static const seq_t period[] = { 1, 2, 22, 2, 1, 44 };

    /* Compute index inside the period. 6 is the period length. */
    idx_t idx = (n - 1) % 6;
    return period[idx];
}
