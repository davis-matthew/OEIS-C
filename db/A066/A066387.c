/*
 * A066387  Triangle T(n,m) (1<=m<=n) giving number of maps f:N -> N such that
 *          f^m(X)=X+n for all natural numbers X.
 * https://oeis.org/A066387
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl, nice
 * Formula:   a(n) = T(r,m) where r(r−1)/2 < n ≤ r(r+1)/2, m = n − r(r−1)/2,
 *            T(r,m) = r!/(r/m)! if m divides r, 0 otherwise
 * Range:     u8 n <= 17, u16 n <= 44, u32 n <= 90, u64 n <= 230,
 *            u128 n <= 629, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                   /* sequence starts at offset 1 */

    /* --- find the row r containing the index n --- */
    idx_t low = 0, high = 2 * n;    /* high is always >= needed r */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        __int128 t = (__int128)mid * (mid + 1) / 2;
        if (t >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;  /* row number */

    /* --- compute column m within that row --- */
    __int128 prev = (__int128)(r - 1) * r / 2;
    idx_t m = n - (idx_t)prev;

    /* --- if r is not divisible by m, the entry is 0 --- */
    if (r % m != 0)
        return 0;

    /* --- compute r! / (r/m)! as a product to avoid large factorials --- */
    idx_t k = r / m;
    seq_t result = 1;
    for (idx_t i = k + 1; i <= r; ++i)
        result *= (seq_t)i;

    return result;
}
