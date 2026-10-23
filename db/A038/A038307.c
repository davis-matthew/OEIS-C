/*
 * A038307  Triangle whose (i,j)-th entry is binomial(i,j)*10^(i-j)*5^j.
 * https://oeis.org/A038307
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = binomial(i, j)*10^(i-j)*5^j, where (i,j) are the row and
 *            column indices in the triangle with entries T(i,j) =
 *            binomial(i,j)*10^(i-j)*5^j
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Helper: integer square root of an unsigned 64‑bit integer.
   Returns floor(sqrt(x)). */
static unsigned long long isqrt_u64(unsigned long long x)
{
    if (x <= 1)
        return x;
    unsigned long long low = 0, high = x, mid, ans = 0;
    while (low <= high) {
        mid = (low + high) >> 1;
        if (mid == 0) {
            low = 1;
            continue;
        }
        unsigned long long div = x / mid;
        if (mid <= div) {   /* mid*mid <= x */
            ans = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break; /* prevent infinite loop */
            high = mid - 1;
        }
    }
    return ans;
}

/* Helper: find the largest i such that i*(i+1)/2 <= n.
   Uses binary search with 128‑bit arithmetic to avoid overflow. */
static unsigned long long find_row(unsigned long long n)
{
    unsigned long long lo = 0, hi = 1;
    while ((unsigned __int128)hi * (hi + 1) / 2 <= n) {
        hi <<= 1;
    }
    while (lo < hi) {
        unsigned long long mid = lo + ((hi - lo + 1) >> 1);
        unsigned __int128 t = (unsigned __int128)mid * (mid + 1) / 2;
        if (t <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* Determine row i and column j */
    unsigned long long i = find_row(n);
    unsigned long long pre = i * (i + 1) / 2;
    unsigned long long j = n - pre;           /* 0 <= j <= i */

    /* Compute binomial(i, j) */
    seq_t res = 1;
    for (unsigned long long k = 1; k <= j; ++k) {
        res = res * (seq_t)(i - k + 1) / (seq_t)k;
    }

    /* Multiply by 10^(i-j) */
    for (unsigned long long e = 0; e < i - j; ++e)
        res *= 10;

    /* Multiply by 5^j */
    for (unsigned long long e = 0; e < j; ++e)
        res *= 5;

    return res;
}
