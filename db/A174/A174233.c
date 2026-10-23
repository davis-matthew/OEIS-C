/*
 * A174233  Triangle T(n,k) read by rows: the numerator of 1/n^2 - 1/(k-n)^2,
 *          0 <= k < 2n.
 * https://oeis.org/A174233
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabf, sign, easy, frac
 * Formula:   Let r be the row with (r‑1)r < n ≤ r(r+1) and * k = n – (r‑1)r –
 *            1 (0‑based column). Then * * a(n) = k*(k‑2r) / gcd( |k*(k‑2r)| ,
 *            r²·(r‑k)² ) , * except a(n) = –1 when k = r (the pole)
 * Range:     i8 n <= 13, i16 n <= 305, i32 n <= 9939, i64 n <= 9939,
 *            i128 n <= 9939, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A small helper for gcd on the generic integer type seq_t. */
static seq_t seq_gcd_local(seq_t a, seq_t b)
{
    if (a < 0) a = -a;
    if (b < 0) b = -b;
    while (b != 0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n < 1)                    /* out of range guard; not needed for OEIS offset */
        return (seq_t)0;

    /* Find the row r with r*(r+1) ≥ n  (and (r‑1)*r < n).  */
    idx_t low = 1, high = 1;
    while ((unsigned __int128)high * (high + 1) < n)
        high <<= 1;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ((unsigned __int128)mid * (mid + 1) >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;                       /* row number */

    idx_t prev   = (unsigned __int128)r * (r - 1); /* terms before row r */
    idx_t offset = n - prev - 1;          /* 0‑based column within the row */

    if (offset == r)                     /* pole k = n */
        return (seq_t)-1;

    seq_t k   = (seq_t)offset;
    seq_t num = k * (k - (seq_t)(2 * r));    /* numerator before reduction */

    if (num == 0)
        return (seq_t)0;

    seq_t sr      = (seq_t)r;
    seq_t diff    = (k > sr) ? (k - sr) : (sr - k); /* |k – r| */
    seq_t diff_sq = diff * diff;
    seq_t n_sq    = sr * sr;
    seq_t denom   = n_sq * diff_sq;              /* r²·(r‑k)² */

    seq_t abs_num = (num < 0) ? -num : num;
    seq_t g       = seq_gcd_local(abs_num, denom);
    seq_t result  = num / g;                     /* reduced numerator */

    return result;
}
