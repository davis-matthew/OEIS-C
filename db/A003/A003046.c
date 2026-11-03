/*
 * A003046  Product of first n Catalan numbers.
 * https://oeis.org/A003046
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = C(0)*C(1)*...*C(n), C(k) = A000108(k), C(0) = 1, C(k+1) =
 *            C(k)*(4k+2)/(k+2); product taken with a balanced product tree
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 7, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Validated: standard, pass, 31 terms through n = 57 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Returns C(lo)*...*C(hi). On entry *c = C(lo); on exit *c = C(hi+1). */
static seq_t a003046_prodcat(idx_t lo, idx_t hi, seq_t *c)
{
    if (lo == hi) {
        seq_t r = *c;
        *c = *c * (seq_t)(4 * lo + 2) / (seq_t)(lo + 2);
        return r;
    }
    idx_t mid = lo + (hi - lo) / 2;
    seq_t left = a003046_prodcat(lo, mid, c);
    seq_t right = a003046_prodcat(mid + 1, hi, c);
    return left * right;
}

seq_t a(idx_t n)
{
    seq_t c = (seq_t)1;
    return a003046_prodcat(0, n, &c);
}
