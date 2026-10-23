/*
 * A048156  Triangular array T read by rows: T(n,k)=k^4 mod n, for
 *          k=1,2,...,n, n=1,2,...
 * https://oeis.org/A048156
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (k^4) mod r, where r is the smallest positive integer
 *            satisfying r(r+1)/2 >= n, and k = n - r(r-1)/2. This flattens
 *            the triangular array T(n,k) = k^4 mod n row by row
 * Range:     u8 n <= 5050, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t find_row(idx_t n)
{
    idx_t low = 1, high = 1;
    /* increase high until it covers n */
    while ((unsigned __int128)high * (high + 1) / 2 < n) high <<= 1;
    /* binary search for minimal r with triangular(r) >= n */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ((unsigned __int128)mid * (mid + 1) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

static seq_t mul_mod_seq(seq_t a, seq_t b, idx_t mod)
{
    if (mod == 0) return 0; /* should not happen */
    return (seq_t)(((unsigned __int128)a * b) % mod);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined below offset, but return 0 for safety */
    idx_t r = find_row(n);
    idx_t prev_tri = (r - 1) * r / 2;
    idx_t k = n - prev_tri;       /* 1 <= k <= r */
    seq_t base = (seq_t)k % r;    /* base <= r-1 */
    /* compute k^4 mod r efficiently: (base^2 mod r)^2 mod r */
    seq_t t = mul_mod_seq(base, base, r);
    seq_t res = mul_mod_seq(t, t, r);
    return res;
}
