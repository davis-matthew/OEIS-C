/*
 * A155856  Triangle T(n,k) = binomial(2*n-k, k)*(n-k)!, read by rows.
 * https://oeis.org/A155856
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = binomial(2*r - k, k) * factorial(r - k) * where r =
 *            floor((sqrt(8*n+1)-1)/2) and k = n - r*(r+1)/2 * (reads the
 *            triangle T(n,k)=C(2n−k,k)*(n−k)! by rows)
 * Range:     u8 n <= 20, u16 n <= 36, u32 n <= 90, u64 n <= 230,
 *            u128 n <= 595, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t binomial(idx_t a, idx_t b)
{
    if (b > a) return 0;
    if (b > a - b) b = a - b;
    seq_t res = 1;
    for (idx_t i = 1; i <= b; ++i) {
        res = res * (a - b + i);
        res = res / i;
    }
    return res;
}

static seq_t factorial(idx_t len)
{
    seq_t res = 1;
    for (idx_t i = 2; i <= len; ++i)
        res *= i;
    return res;
}

seq_t a(idx_t n)
{
    /* Find row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2. */
    idx_t low  = 0, high = n;
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        unsigned __int128 off = (__int128)mid * (mid + 1) / 2;
        if (off <= n)
            low = mid;
        else
            high = mid - 1;
    }
    idx_t r = low;

    unsigned __int128 offsetR = (__int128)r * (r + 1) / 2;
    idx_t k = n - (idx_t)offsetR;  /* column in the row */

    seq_t bin  = binomial(2 * r - k, k);
    seq_t fact = factorial(r - k);

    return bin * fact;
}
