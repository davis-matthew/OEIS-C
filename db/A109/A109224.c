/*
 * A109224  Minimum number of moves to solve the second Panex puzzle of order
 *          n of exchanging the two side towers.
 * https://oeis.org/A109224
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  hard, more, nonn
 * Formula:   a(n) = 49*A_n - 56*B_n - 8*n - 2 (for n >= 5) where (1+√2)^n =
 *            A_n + B_n√2 and A_0=1, B_0=0, A_{k+1} = A_k + 2*B_k, B_{k+1} =
 *            A_k + B_k. For n = 1..4 the terms are 3, 13, 42, 128
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Return known values for n = 1..4 */
    if (n < 5) {
        switch (n) {
            case 1: return (seq_t)3;
            case 2: return (seq_t)13;
            case 3: return (seq_t)42;
            case 4: return (seq_t)128;
            default: /* n == 0 or n < 0 is not expected */ return (seq_t)0;
        }
    }

    /* Compute A_n and B_n by the recurrence */
    seq_t A = 1;  /* A_0 */
    seq_t B = 0;  /* B_0 */

    for (idx_t i = 0; i < n; ++i) {
        seq_t nextA = A + (seq_t)2 * B;
        seq_t nextB = A + B;
        A = nextA;
        B = nextB;
    }

    /* Apply the closed-form formula */
    seq_t t = (seq_t)49 * A - (seq_t)56 * B;
    t -= (seq_t)8 * n;
    t -= (seq_t)2;

    return t;
}
