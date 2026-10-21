/*
 * A207937  Number of n X 7 0..1 arrays avoiding 0 0 1 and 1 0 0 horizontally
 *          and 0 0 1 and 1 1 0 vertically.
 * https://oeis.org/A207937
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = (1260 + 4974*n - 32165*n^2 - 6041*n^3 + 51940*n^4 +
 *            21091*n^5 * + 2905*n^6 + 136*n^7) / 1260 (n ≥ 1) * * For n ≤ 8
 *            the values are taken from the OEIS list. For larger n the *
 *            polynomial is evaluated in a signed 128‑bit integer to avoid
 *            overflow * and then cast to seq_t. This works for an...
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 29, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not defined for n < 1 */

    /* Pre‑computed first eight terms */
    static const seq_t base[8] = {
        35ULL,      /* a(1) */
        1225ULL,    /* a(2) */
        8977ULL,    /* a(3) */
        38207ULL,   /* a(4) */
        121313ULL,  /* a(5) */
        319439ULL,  /* a(6) */
        737575ULL,  /* a(7) */
        1544037ULL  /* a(8) */
    };
    if (n <= 8) return base[n-1];

    /* Compute n^k (k = 1..7) in unsigned 128 bits */
    unsigned __int128 t = (unsigned __int128)n;
    unsigned __int128 p2 = t * t;          /* n^2 */
    unsigned __int128 p3 = p2 * t;         /* n^3 */
    unsigned __int128 p4 = p3 * t;         /* n^4 */
    unsigned __int128 p5 = p4 * t;         /* n^5 */
    unsigned __int128 p6 = p5 * t;         /* n^6 */
    unsigned __int128 p7 = p6 * t;         /* n^7 */

    /* Evaluate the polynomial using a signed 128‑bit integer */
    __int128 num = 0;
    num += (__int128)136  * (__int128)p7;
    num += (__int128)2905 * (__int128)p6;
    num += (__int128)21091 * (__int128)p5;
    num += (__int128)51940 * (__int128)p4;
    num -= (__int128)6041  * (__int128)p3;
    num -= (__int128)32165 * (__int128)p2;
    num += (__int128)4974  * (__int128)t;
    num += (__int128)1260;

    /* Division by 1260 is exact (OEIS guarantee) */
    return (seq_t)(num / 1260);
}
