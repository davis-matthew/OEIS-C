/*
 * A107917  a(n) = (n+1)(n+2)^2*(n+3)^3*(n+4)^2*(n+5)(n^2 + 6n + 10)/86400.
 * https://oeis.org/A107917
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)(n+2)^2(n+3)^3(n+4)^2(n+5)(n^2 + 6n + 10)/86400
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 18, u64 n <= 155, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline void reduce_factor(idx_t *f, idx_t *e2, idx_t *e3, idx_t *e5)
{
    /* Cancel as many 2's as possible from f */
    while (*e2 > 0 && (*f % (idx_t)2U) == 0) {
        *f /= (idx_t)2U;
        *e2  -= (idx_t)1U;
    }
    /* Cancel as many 3's as possible from f */
    while (*e3 > 0 && (*f % (idx_t)3U) == 0) {
        *f /= (idx_t)3U;
        *e3  -= (idx_t)1U;
    }
    /* Cancel as many 5's as possible from f */
    while (*e5 > 0 && (*f % (idx_t)5U) == 0) {
        *f /= (idx_t)5U;
        *e5  -= (idx_t)1U;
    }
}

seq_t a(idx_t n)
{
    /* Denominator prime exponents for 86400 = 2^7 * 3^3 * 5^2 */
    idx_t e2 = (idx_t)7U;
    idx_t e3 = (idx_t)3U;
    idx_t e5 = (idx_t)2U;

    seq_t result = (seq_t)1U;

    /* 1) (n+1) */
    {
        idx_t f = n + (idx_t)1U;
        reduce_factor(&f, &e2, &e3, &e5);
        result *= (seq_t)f;
    }

    /* 2) (n+2)^2 */
    for (int i = 0; i < 2; ++i) {
        idx_t f = n + (idx_t)2U;
        reduce_factor(&f, &e2, &e3, &e5);
        result *= (seq_t)f;
    }

    /* 3) (n+3)^3 */
    for (int i = 0; i < 3; ++i) {
        idx_t f = n + (idx_t)3U;
        reduce_factor(&f, &e2, &e3, &e5);
        result *= (seq_t)f;
    }

    /* 4) (n+4)^2 */
    for (int i = 0; i < 2; ++i) {
        idx_t f = n + (idx_t)4U;
        reduce_factor(&f, &e2, &e3, &e5);
        result *= (seq_t)f;
    }

    /* 5) (n+5) */
    {
        idx_t f = n + (idx_t)5U;
        reduce_factor(&f, &e2, &e3, &e5);
        result *= (seq_t)f;
    }

    /* 6) (n^2 + 6n + 10) */
    {
        idx_t f = n * n + (idx_t)6U * n + (idx_t)10U;
        reduce_factor(&f, &e2, &e3, &e5);
        result *= (seq_t)f;
    }

    /* Any remaining denominator factors (should be zero) */
    while (e2 > 0) { result /= (seq_t)2U; e2 -= (idx_t)1U; }
    while (e3 > 0) { result /= (seq_t)3U; e3 -= (idx_t)1U; }
    while (e5 > 0) { result /= (seq_t)5U; e5 -= (idx_t)1U; }

    return result;
}
