/*
 * A051032  Summatory Rudin-Shapiro sequence for 2^(n-1).
 * https://oeis.org/A051032
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(floor(n/2)) + 1
 * Range:     u8 n <= 15, u16 n <= 31, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A051032:  Summatory Rudin–Shapiro sequence for 2^(n‑1)   */
/*  a(1) = 2, a(2) = 3, a(3) = 3, a(4) = 5, …                      */
seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1.  For n = 1 we have */
    /* floor(1/2) = 0 → 2^0 + 1 = 2.                      */
    /* For general n the exponent is floor(n/2).           */

    idx_t e = n / 2;            /* floor(n/2) – integer division */
    seq_t val = (seq_t)1;       /* start with 2^0 */

    /* Multiply by 2 e times to obtain 2^e.            */
    /* Works for unsigned small integers (≤64‑bit)   */
    /* and for arbitrary‑precision types that support  */
    /* the '*' operator.                                */
    for (idx_t i = 0; i < e; ++i)
        val *= (seq_t)2;

    return val + (seq_t)1;     /* 2^e + 1 */
}
