/*
 * A117075  Number at end of segment n of A117073.
 * https://oeis.org/A117075
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(1) = 3, a(2) = 6, a(3) = 11, a(4) = 22 * for n ≥ 5 : * a(n)
 *            = 42·2^(n‑5) – 2·2^(n‑10) (the second term is 0 when n < 10) *
 *            The sequence thus doubles each step after n = 5, except a(n) for
 *            n ≥ 10 * is reduced by 2·2^(n‑10) because the 10‑th segment ends
 *            one number * earlier than the pattern wo...
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 17, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the unusually small first four terms */
    switch (n)
    {
        case 1:  return (seq_t)3;
        case 2:  return (seq_t)6;
        case 3:  return (seq_t)11;
        case 4:  return (seq_t)22;
        default: break;
    }

    /* n ≥ 5 */
    seq_t val = (seq_t)42;
    for (idx_t i = 5; i < n; ++i)
        val *= 2;

    if (n >= 10)
    {
        seq_t sub = (seq_t)2;
        for (idx_t i = 10; i < n; ++i)
            sub *= 2;
        val -= sub;
    }

    return val;
}
