/*
 * A027917  a(n) = least k such that 1+2+...+k >= E{1,2,...,n}, where E is the
 *          3rd elementary symmetric function.
 * https://oeis.org/A027917
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min{k | k(k+1)/2 >= n^2 (n+1)^2 (n-1)(n-2)/48} (offset 3)
 * Range:     u8 n <= 10, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>
#include <stddef.h>

/* use native 128-bit type for intermediate arithmetic (available with GCC/Clang) */
typedef unsigned __int128 u128;

seq_t a(idx_t n)
{
    /* for n < 3 the sequence is undefined but we return 0 */
    if (n < 3) return 0;

    /* compute e3(n) = n^2 (n+1)^2 (n-1)(n-2) / 48   (see derivation) */
    u128 nn = n;
    u128 num = nn * nn * (nn + 1) * (nn + 1) * (nn - 1) * (nn - 2);
    num /= (u128)48;               /* e3(n) fits in u128 */

    /* target for quadratic inequality: k(k+1)/2 >= num  */
    /* double estimate of k using the inverse of k(k+1)/2 ≈ n^2/2  */
    long double approx = sqrtl(4.0L * (long double)num + 1.0L);
    uint64_t k_est = (uint64_t)((approx - 1.0L) / 2.0L + 0.5L); /* round to nearest */

    /* adjust k_est downwards if too large */
    while (k_est > 0) {
        u128 tri = (u128)k_est * (k_est + 1) / 2;
        if (tri < num) break;
        if (k_est == 0) break;
        k_est--;
    }
    /* adjust upwards if still too small */
    while (1) {
        u128 tri = (u128)k_est * (k_est + 1) / 2;
        if (tri >= num) break;
        k_est++;
    }

    return (seq_t)k_est;
}
