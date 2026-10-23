/*
 * A147883  Number of consistent sets of 9 irreflexive binary order
 *          relationships over n objects.
 * https://oeis.org/A147883
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-4)*(n-3)*(n-2)*(n-1)*n*(n^13 + n^12 - 61*n^11 -
 *            271*n^10 + 501*n^9 + 10539*n^8 + 69721*n^7 + 170899*n^6 -
 *            1975510*n^5 - 21334612*n^4 - 30150228*n^3 + 619527780*n^2 +
 *            1942605000*n - 10974342960)/362880
 * Range:     u8 none, u16 none, u32 n <= 6, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t seq_gcd_local(seq_t a, seq_t b)
{
    while (b != 0)
    {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n < 5)
        return 0;

    /* cast the index */
    seq_t sn = (seq_t)n;

    /* ----- compute C(n,5) = n*(n-1)*(n-2)*(n-3)*(n-4)/120 ----- */
    seq_t comb = 1;
    seq_t denomC = (seq_t)120;          /* 5! */

    seq_t factors[5] = { sn - 4, sn - 3, sn - 2, sn - 1, sn };
    for (int i = 0; i < 5; ++i)
    {
        comb *= factors[i];
        seq_t g = seq_gcd_local(comb, denomC);
        comb /= g;
        denomC /= g;
    }
    /* after the loop denomC should be 1 */

    /* ----- pre‑compute powers of n up to n^13 ----- */
    seq_t pow[14];
    pow[0] = 1;
    for (int i = 1; i <= 13; ++i)
        pow[i] = pow[i - 1] * sn;

    /* ----- evaluate the 13‑degree polynomial P(n) ----- */
    seq_t P =
        pow[13] + pow[12] -
        (seq_t)61   * pow[11] -
        (seq_t)271  * pow[10] +
        (seq_t)501  * pow[ 9] +
        (seq_t)10539 * pow[ 8] +
        (seq_t)69721 * pow[ 7] +
        (seq_t)170899 * pow[ 6] -
        (seq_t)1975510 * pow[ 5] -
        (seq_t)21334612 * pow[ 4] -
        (seq_t)30150228 * pow[ 3] +
        (seq_t)619527780 * pow[ 2] +
        (seq_t)1942605000 * pow[ 1] -
        (seq_t)10974342960;

    /* ----- remaining denominator 3024 = 362880 / 120 ----- */
    seq_t denom = (seq_t)3024;

    /* cancel factors of denom with comb and P */
    seq_t g = seq_gcd_local(comb, denom);
    comb /= g;
    denom /= g;

    g = seq_gcd_local(P, denom);
    P /= g;
    denom /= g;

    /* final multiplication (denom should be 1) */
    seq_t result = comb * P;
    if (denom != (seq_t)1)
        result /= denom;

    return result;
}
