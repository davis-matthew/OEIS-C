/*
 * A143355  Triangle read by rows: T(n,k) = Sum_{d|n} floor(d/k).
 * https://oeis.org/A143355
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where T(r,k)=∑_{d|r} ⌊d/k⌋, and (r,k) is obtained
 *            from the row‑major index n: r = smallest integer with r·(r+1)/2
 *            ≥ n k = n – (r–1)·r/2 (1 ≤ k ≤ r)
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

/*--- internal helpers -----------------------------------------------------*/

static inline unsigned long long
row_from_index(unsigned long long n)
/* Compute the row r such that n ≤ r·(r+1)/2   (1‑based). */
{
    /* initial estimate via double arithmetic, then correct */
    long double ld = (long double)n;
    unsigned long long r = (unsigned long long)
        (floorl((sqrtl(8.0L*ld + 1.0L) - 1.0L) / 2.0L) + 0.5L);

    /* Correct possible off‑by‑one due to rounding */
    while ((unsigned __int128)r*(r+1)/2 < n) r++;
    while (r > 1 && (unsigned __int128)(r-1)*r/2 >= n) r--;
    return r;
}

/*--- exposed function -----------------------------------------------------*/

seq_t
a(idx_t n)
{
    /* n is 1‑based.  Handle the minimal case explicitly. */
    if (n == 0)
        return (seq_t)0;   /* out of range, but harmless */

    /* Determine row and column */
    unsigned long long r = row_from_index((unsigned long long)n);

    /* preceding elements count = (r-1)*r/2 */
    unsigned long long prev = (unsigned long long)((unsigned __int128)(r-1) * r / 2);
    unsigned long long k   = (unsigned long long)n - prev;   /* 1 ≤ k ≤ r */

    /* Sum over all divisors d of r of floor(d/k) */
    seq_t sum = (seq_t)0;
    for (unsigned long long d = 1; (__int128)d * d <= r; ++d) {
        if (r % d == 0) {
            unsigned long long d1 = d;
            unsigned long long d2 = r / d;

            sum += (seq_t)d1 / (seq_t)k;
            if (d2 != d1)
                sum += (seq_t)d2 / (seq_t)k;
        }
    }
    return sum;
}
