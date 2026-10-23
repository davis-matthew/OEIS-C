/*
 * A018512  Divisors of 548.
 * https://oeis.org/A018512
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the nth smallest divisor of 548, sorted ascending
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Return 0 for indices outside the finite set 1..6. */
    if (n < 1 || n > 6) return (seq_t)0;

    /* 548 = 2^2 * 137^1 => small fixed set of divisors. */
    static const seq_t pow2[3]  = { (seq_t)1, (seq_t)2, (seq_t)4 };
    static const seq_t pow137[2] = { (seq_t)1, (seq_t)137 };

    seq_t div[6];
    int idx = 0;

    /* Generate all divisors. */
    for (int e2 = 0; e2 < 3; ++e2)
        for (int e137 = 0; e137 < 2; ++e137)
            div[idx++] = pow2[e2] * pow137[e137];

    /* Simple sorting (bubble sort for such a tiny array). */
    for (int i = 0; i < 5; ++i)
        for (int j = i + 1; j < 6; ++j)
            if (div[j] < div[i]) {
                seq_t tmp = div[i];
                div[i] = div[j];
                div[j] = tmp;
            }

    /* Return the requested term. */
    return div[(int)(n - 1)];
}
