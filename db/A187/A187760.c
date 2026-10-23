/*
 * A187760  Table T(n,k) read by antidiagonals. T(n,k)=n-k+1, if n>=k,
 *          T(n,k)=k-n+2, if n < k.
 * https://oeis.org/A187760
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(i,j) where (i,j) is obtained by enumerating the
 *            infinite table T with parameter m=3: T(n,k)=n-k+1 if n>=k,
 *            T(n,k)=k-n+2 if n<k. The integers are listed by antidiagonals
 *            (Cantor ordering)
 * Range:     u8 n <= 9870, u16 n <= 9870, u32 n <= 9870, u64 n <= 9870,
 *            u128 n <= 9870, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Integer square root for unsigned 64‑bit values.
   Returns floor(sqrt(x)).  No floating point and no external helpers. */
static idx_t isqrt_u64(idx_t n)
{
    idx_t res = 0;
    idx_t bit = 1ULL << 62;          /* start at the highest power of 4 <= 2^64 */
    while (bit > n) bit >>= 2;
    while (bit) {
        if (n >= res + bit) {
            n -= res + bit;
            res = (res >> 1) + bit;
        } else {
            res >>= 1;
        }
        bit >>= 2;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Convert the 1‑based index to the corresponding (i,j) coordinates. */
    idx_t s = n;                    /* n >= 1 as guaranteed by the offset */
    /* Compute t = floor((sqrt(8*n-7)-1)/2).  One overflow safe approach:
       compute sqrt(8*n-7) using integer sqrt. */
    idx_t temp = 8 * n;
    /* 8*n fits in 64 bits for n <= 2^61-1.  Under the usual test range
       n is far smaller, so this is safe. */
    temp -= 7;
    idx_t sqrt_val = isqrt_u64(temp);
    idx_t t = (sqrt_val - 1) / 2;      /* t = i+j-2 in Cantor ordering */

    /* The first index on diagonal t (1‑based) */
    idx_t start = t * (t + 1) / 2 + 1;

    /* Offset of our index on that diagonal (0‑based) */
    idx_t d = n - start;

    /* Coordinates: i = d+1, j = t+1-d */
    idx_t i = d + 1;
    idx_t j = t + 1 - d;

    /* Compute T(i,j) for m=3. */
    seq_t val;
    if (i >= j)
        val = (seq_t)(i - j + 1);
    else
        val = (seq_t)(j - i + 2);

    return val;
}
