/*
 * A202095  Number of (n+2)X5 binary arrays avoiding patterns 001 and 011 in
 *          rows and columns.
 * https://oeis.org/A202095
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+4)^4*(n+6)^4*(n+8)^2/147456 for n even, a(n) =
 *            (n+3)^2*(n+5)^4*(n+7)^3*(n+9)/147456 for n odd
 * Range:     u8 none, u16 n <= 4, u32 n <= 24, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Validated: standard, pass, 45 terms through n = 196 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t compute_using_128(idx_t n)
{
    unsigned __int128 num = 1;
    if ((n & 1) == 0) {                   /* even n */
        for (int i = 0; i < 4; ++i) num *= (unsigned __int128)(n + 4);
        for (int i = 0; i < 4; ++i) num *= (unsigned __int128)(n + 6);
        for (int i = 0; i < 2; ++i) num *= (unsigned __int128)(n + 8);
    } else {                               /* odd n */
        for (int i = 0; i < 2; ++i) num *= (unsigned __int128)(n + 3);
        for (int i = 0; i < 4; ++i) num *= (unsigned __int128)(n + 5);
        for (int i = 0; i < 3; ++i) num *= (unsigned __int128)(n + 7);
        num *= (unsigned __int128)(n + 9);
    }
    num /= 147456;
    return (seq_t)num;
}

static seq_t compute_using_bigint(idx_t n)
{
    seq_t num = 1;
    if (n & 1) {                         /* odd n */
        for (int i = 0; i < 2; ++i) num = num * (n + 3);
        for (int i = 0; i < 4; ++i) num = num * (n + 5);
        for (int i = 0; i < 3; ++i) num = num * (n + 7);
        num = num * (n + 9);
    } else {                               /* even n */
        for (int i = 0; i < 4; ++i) num = num * (n + 4);
        for (int i = 0; i < 4; ++i) num = num * (n + 6);
        for (int i = 0; i < 2; ++i) num = num * (n + 8);
    }
    return num / 147456;
}

seq_t a(idx_t n)
{
    /* Use 128‑bit intermediate only when seq_t is small (≤64 bits). */
    if (sizeof(seq_t) <= 8)
        return compute_using_128(n);
    else
        return compute_using_bigint(n);
}
