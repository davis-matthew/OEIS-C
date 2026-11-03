/*
 * A145911  a(n) = A145909(n)/8.
 * https://oeis.org/A145911
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A106619(n) * A106619(n+1) * where * A106619(n) = n if n
 *            is odd and not divisible by 3 * = n/2 if n is even and not
 *            divisible by 3 * = n / (3^e) if n ≡ 0 (mod 3), n odd, * with e =
 *            1 if v₃(n)=1 or 2, else e = 2 * = n / (2·3^e) if n ≡ 0 (mod 3),
 *            n even, * with e as above * (here v₃(n) is the...
 * Range:     u8 n <= 24, u16 n <= 363, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 69 terms through n = 4890 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t _a106619(idx_t m)
{
    if (m == 0)
        return 0;

    /* If n is not divisible by 3 */
    if ((m % 3) != 0) {
        if ((m & 1) == 0)   /* even */
            return (seq_t)m / 2;
        else                /* odd */
            return (seq_t)m;
    }

    /* n is divisible by 3 */
    unsigned int e_cap = (m % 9 == 0) ? 2u : 1u;     /* e_cap = min(v3(n),2) */
    seq_t d3 = (e_cap == 1u) ? 3u : 9u;              /* 3^e_cap */
    seq_t denom = (m & 1) ? d3 : d3 * 2u;           /* even -> 2*3^e_cap */

    return (seq_t)m / denom;
}

seq_t a(idx_t n)
{
    return _a106619(n) * _a106619(n + 1);
}
