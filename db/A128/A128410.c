/*
 * A128410  A128408 * A000012.
 * https://oeis.org/A128410
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, sign
 * Formula:   Let r be the unique integer with r(r+1)/2 ≥ n. Let k = n –
 *            r(r–1)/2. Then a(n) = μ(r) * number of divisors of r that are ≥
 *            k
 * Range:     i8 n <= 55, i16 n <= 55, i32 n <= 55, i64 n <= 55, i128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the row r containing the n‑th entry. */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n) {
        ++r;
    }

    /* Position inside the row (1‑based). */
    idx_t prev = (r - 1) * r / 2;
    idx_t k = n - prev;    /* 1 ≤ k ≤ r */

    /* Compute the Möbius function μ(r). */
    idx_t tmp = r;
    int distinct = 0;
    int mu_val = 1;
    for (idx_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            int exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }
            if (exp > 1) {          /* r is not square‑free */
                mu_val = 0;
                break;
            }
            ++distinct;
        }
    }
    if (mu_val != 0 && tmp > 1) {
        /* One remaining prime factor. */
        ++distinct;
    }
    if (mu_val != 0) {
        mu_val = (distinct & 1) ? -1 : 1;
    }

    if (mu_val == 0) {
        return (seq_t)0;
    }

    /* Count divisors of r that are at least k.  r is small for n ≤ 10 000. */
    idx_t cnt = 0;
    for (idx_t d = k; d <= r; ++d) {
        if (r % d == 0) {
            ++cnt;
        }
    }

    return (seq_t)mu_val * (seq_t)cnt;
}
