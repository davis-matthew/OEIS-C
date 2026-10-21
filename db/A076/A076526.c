/*
 * A076526  a(n) = r * max(e_1, ..., e_r), where n = p_1^e_1 . .... p_r^e_r is
 *          the canonical prime factorization of n, a(1) = 0.
 * https://oeis.org/A076526
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = r * max(e_1, ..., e_r)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1) return 0;

    idx_t tmp = n;
    seq_t distinct = 0;
    seq_t max_exp = 0;

    for (idx_t p = 2; p <= tmp / p; ++p) {
        if (tmp % p == 0) {
            ++distinct;
            seq_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            if (e > max_exp) max_exp = e;
        }
    }

    if (tmp > 1) {          /* remaining prime factor */
        ++distinct;
        if ((seq_t)1 > max_exp) max_exp = 1;
    }

    return distinct * max_exp;
}
