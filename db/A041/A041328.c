/*
 * A041328  Numerators of continued fraction convergents to sqrt(178).
 * https://oeis.org/A041328
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 32, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A041328: Numerators of continued fraction convergents to sqrt(178)
 *
 * Let a_0 = 13 and the periodic part of the continued fraction of sqrt(178)
 * be [2, 1, 12, 1, 2, 26] (period length 6).  Define the sequence {p_n}
 * by the recurrence
 *
 *   p_{-2} = 0,   p_{-1} = 1
 *   p_n   = a_n * p_{n-1} + p_{n-2}   for n ≥ 0
 *
 * where a_n = a_0 for n = 0 and a_n = period[(n-1) mod 6] for n ≥ 1.
 *
 * Then a(n) = p_n.
 *
 * This implementation computes p_n directly in O(n) time using only
 * the +, -, *, and modulo operators, suitable for all allowed
 * built 2‑architecture integer types.
 */
seq_t a(idx_t n)
{
    const seq_t a0 = 13;
    static const seq_t period[6] = {2, 1, 12, 1, 2, 26};

    seq_t p_m2 = 0;   /* p_{i-2} */
    seq_t p_m1 = 1;   /* p_{i-1} */
    seq_t p_cur = 0;  /* p_i */

    for (idx_t i = 0; i <= n; ++i) {
        seq_t a_cur = (i == 0) ? a0 : period[(i - 1) % 6];
        p_cur = a_cur * p_m1 + p_m2;
        if (i == n)
            return p_cur;
        p_m2 = p_m1;
        p_m1 = p_cur;
    }
    return 0; /* unreachable */
}
