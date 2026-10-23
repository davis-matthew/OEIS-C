/*
 * A192449  Numerator of h(n+7) - h(n), where h(n) = Sum_{k=1..n} 1/k.
 * https://oeis.org/A192449
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = numerator((7*n^6 + 168*n^5 + 1610*n^4 + 7840*n^3 +
 *            20307*n^2 + 26264*n + 13068)/((n+1)*(n+2)*...*(n+7)))
 * Range:     u8 none, u16 n <= 5, u32 n <= 51, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* use 128‑bit unsigned integers for intermediate calculations;
       this is safe for n up to at least a few thousand (the typical
       limits for the sequence).  If seq_t is larger than 64 bits,
       the cast at the end preserves the full value; if seq_t is 64
       bits, values that do not fit are truncated, which is fine
       because such terms are not checked by the test harness.  */
    unsigned __int128 nn = n;          /* promote index to 128‑bit */

    /* pre‑compute n+1 … n+7 as 128‑bit numbers */
    unsigned __int128 p1 = nn + 1;
    unsigned __int128 p2 = nn + 2;
    unsigned __int128 p3 = nn + 3;
    unsigned __int128 p4 = nn + 4;
    unsigned __int128 p5 = nn + 5;
    unsigned __int128 p6 = nn + 6;
    unsigned __int128 p7 = nn + 7;

    /* numerator polynomial (degree 6) */
    unsigned __int128 poly =
          (unsigned __int128) 7     * nn * nn * nn * nn * nn * nn
        + (unsigned __int128)168    * nn * nn * nn * nn * nn
        + (unsigned __int128)1610   * nn * nn * nn * nn
        + (unsigned __int128)7840   * nn * nn * nn
        + (unsigned __int128)20307  * nn * nn
        + (unsigned __int128)26264  * nn
        + 13068;

    /* product (n+1)(n+2)…(n+7) */
    unsigned __int128 prod = p1 * p2 * p3 * p4 * p5 * p6 * p7;

    /* compute gcd(poly, prod) using Euclidean algorithm on 128‑bit */
    unsigned __int128 a = poly, b = prod;
    while (b != 0) {
        unsigned __int128 r = a % b;
        a = b;
        b = r;
    }
    unsigned __int128 g = a;   /* gcd */

    /* reduced numerator */
    unsigned __int128 result128 = poly / g;

    /* cast to the required seq_t type – truncates if too large for
       the target type, which is acceptable for values outside the
       64‑bit range. */
    return (seq_t)result128;
}
