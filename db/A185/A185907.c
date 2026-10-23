/*
 * A185907  Weight array of A185908, by descending antidiagonals.
 * https://oeis.org/A185907
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   let S be the smallest integer with S*(S-1)/2 >= n. Let i = n -
 *            (S-2)*(S-1)/2. Then a(n) = 1 if i = S-1 (first column) or i =
 *            S/2 (main diagonal, which occurs when S is even), else a(n)=0
 * Range:     u8 n <= 1275, u16 n <= 1275, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* integer square root of an unsigned 128‑bit value */
static idx_t isqrt_u128(unsigned __int128 x)
{
    idx_t low = 0;
    /* high large enough for sqrt(1+8*n) while n < 2^63 */
    idx_t high = ((idx_t)1 << 34); /* 2^34 > sqrt(2^64) */
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if ((unsigned __int128)mid * mid <= x)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n == 0) /* offset is 1; return 0 for safety */
        return (seq_t)0;

    /* find smallest S with S*(S-1)/2 >= n */
    unsigned __int128 val = (unsigned __int128)1 + (unsigned __int128)8 * n;
    idx_t d = isqrt_u128(val);                /* floor(sqrt(1+8*n)) */
    idx_t S = (d + 1) >> 1;                   /* floor((sqrt(1+8*n)+1)/2) */
    if ((unsigned __int128)S * (S - 1) / 2 < n)
        ++S;                                   /* ensure S satisfies the inequality */

    /* number of terms before this diagonal */
    unsigned __int128 prev = (unsigned __int128)(S - 2) * (S - 1) / 2;
    idx_t offset = n - (idx_t)prev - 1;        /* 0‑based index within the diagonal */
    idx_t i = offset + 1;                     /* row index (1‑based) */
    idx_t j = S - i;                          /* column index */

    return (seq_t)((j == 1) || (i == j));
}
