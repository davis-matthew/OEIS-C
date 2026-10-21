/*
 * A230864  log2*(n) (version 3): number of iterations
 *          log_2(log_2(log_2(...(n)...))) required for the result to be <= 1.
 * https://oeis.org/A230864
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of iterations of log₂ needed to bring a number *
 *            down to ≤ 1. Equivalently, a(n) is the smallest k ≥ 0 such that
 *            * n ≤ 2↑↑k (power tower of k copies of 2). For n ≤ 2⁴⁰ (i.e.
 *            2¹⁶) the * value is at most 5, so a simple threshold test
 *            suffices
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* external linkage */
seq_t a(idx_t n)
{
    if (n == 1) return 0;        /* a(1) = 0 */
    if (n == 2) return 1;        /* a(2) = 1 */
    if (n <= 4)  return 2;       /* 3–4  → 2 */
    if (n <= 16) return 3;       /* 5–16 → 3 */
    if (n <= 65536) return 4;    /* 17–65536 → 4 */
    return 5;                    /* all larger 64‑bit values → 5 */
}
