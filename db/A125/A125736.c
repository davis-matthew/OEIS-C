/*
 * A125736  a(n)=IntegerLog(n-1)+IntegerLog(n+1), where
 *          IntegerLog(n)=A001414(n).
 * https://oeis.org/A125736
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = IntegerLog(n-1) + IntegerLog(n+1) IntegerLog(m) = sum
 *            over prime factors p^e of (p*e) (sequence A001414)
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Validated: partial, pass, 70 terms through n = 72 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t integer_log(idx_t m)
{
    seq_t sum = 0;
    idx_t exp;

    /* count factor 2 */
    exp = 0;
    while ((m & 1ULL) == 0) {
        exp++;
        m >>= 1;
    }
    if (exp) sum += (seq_t)2 * exp;

    /* odd factors */
    for (idx_t p = 3; p <= m / p; p += 2) {
        exp = 0;
        while (m % p == 0) {
            exp++;
            m /= p;
        }
        if (exp) sum += (seq_t)p * exp;
    }

    /* remaining prime > 1 */
    if (m > 1) sum += (seq_t)m;

    return sum;
}

seq_t a(idx_t n)
{
    /* n >= 3 as per sequence offset */
    return integer_log(n - 1) + integer_log(n + 1);
}
