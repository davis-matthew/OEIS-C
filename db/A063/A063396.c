/*
 * A063396  T(3,n) with T(n,m) as in A063394.
 * https://oeis.org/A063396
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1/4 * [(n+1)(n+2)2^n + 10(n+1)2^n + 6·2^n – 4] for n > 0,
 *            a(0) = 1. Equivalent simplified form for n ≥ 2 is: a(n) =
 *            2^(n-2) * (n^2 + 13n + 18) – 1
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 24, u64 n <= 54, u128 n <= 116,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS A063396. */
seq_t a(idx_t n)
{
    if (n == 0) return 1;          /* a(0) = 1 */

    /* For n == 1 use the full formula to avoid under‑flow */
    if (n == 1) {
        seq_t pow2 = 1;
        pow2 *= 2;                      /* 2^1 */
        seq_t tmp = (seq_t)1 * (seq_t)1 + (seq_t)13 * (seq_t)1 + (seq_t)18;  /* 32 */
        return (pow2 * tmp - 4) / 4;    /* (2*32 - 4)/4 = 15 */
    }

    /* For n ≥ 2: a(n) = 2^(n-2)*(n^2 + 13n + 18) – 1  */
    seq_t pow2_shift = 1;                /* 2^(n-2) */
    for (idx_t i = 0; i < n - 2; ++i) {
        pow2_shift *= 2;
    }

    seq_t nn   = (seq_t)n;
    seq_t tmp  = nn * nn + (seq_t)13 * nn + (seq_t)18;   /* n^2 + 13n + 18 */

    return pow2_shift * tmp - 1;
}
