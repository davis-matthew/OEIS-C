/*
 * A127099  Triangle T(n,m) = A126988 *A127093 read by rows.
 * https://oeis.org/A127099
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the (n‑th) entry of the triangle T, where T(r,c) = 0 if
 *            c ∤ r, and otherwise T(r,c) = c * σ(r/c) (σ = sum of divisors).
 *            The sequence is obtained by reading the triangle by rows (offset
 *            1)
 * Range:     u8 n <= 95, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Return the sum of divisors of a positive integer n. */
static seq_t sigma(uint64_t n)
{
    seq_t sum = 0;
    for (uint64_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            sum += (seq_t)i;
            uint64_t j = n / i;
            if (j != i) sum += (seq_t)j;
        }
    }
    return sum;
}

/* a(n) -- required external linkage */
seq_t a(idx_t N)
{
    if (N < 1) return 0;                 /* undefined for n<1 */
    /* Find smallest r such that r*(r+1)/2 >= N */
    uint64_t low = 1, high = 2;
    while ((unsigned __int128)high * (high + 1) / 2 < N) high <<= 1;
    while (low < high) {
        uint64_t mid = (low + high) >> 1;
        if ((unsigned __int128)mid * (mid + 1) / 2 >= N)
            high = mid;
        else
            low = mid + 1;
    }
    uint64_t r = low;                    /* row index */
    uint64_t triangular_prev = (unsigned __int128)(r - 1) * r / 2;  /* (r-1)*r/2 */
    uint64_t c = N - triangular_prev;    /* column index, 1 <= c <= r */

    if (r % c != 0) return 0;            /* not a divisor, entry is 0 */

    uint64_t divisor = r / c;            /* r/c is integer */
    seq_t sigma_val = sigma(divisor);
    return (seq_t)c * sigma_val;
}
