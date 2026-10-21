/*
 * A214685  a(n) is obtained from n by removing 2s, 3s, and 5s from the prime
 *          factorization of n that do not contribute to a factor of 30.
 * https://oeis.org/A214685
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n*30^(v_30(n)))/(2^(v_2(n))*3^(v_3(n))*5^(v_5(n)))
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Extract exponents of 2, 3, 5 in n. */
    idx_t temp = n;
    idx_t e2 = 0, e3 = 0, e5 = 0;
    while (temp % 2 == 0) { temp /= 2; e2++; }
    while (temp % 3 == 0) { temp /= 3; e3++; }
    while (temp % 5 == 0) { temp /= 5; e5++; }

    /* Minimum of the three exponents gives v_30(n). */
    idx_t e0 = e2 < e3 ? e2 : e3;
    if (e5 < e0) e0 = e5;

    /* Remove surplus 2, 3 and 5 factors. */
    seq_t result = (seq_t)n;
    idx_t t;

    t = e2 - e0;
    while (t--) result /= 2;
    t = e3 - e0;
    while (t--) result /= 3;
    t = e5 - e0;
    while (t--) result /= 5;

    return result;
}
