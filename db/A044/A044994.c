/*
 * A044994  Numbers whose base-3 representation contains exactly one 0 and no
 *          2's.
 * https://oeis.org/A044994
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = numbers whose base‑3 representation contains exactly one
 *            0 and no 2's. a(1) = 0; for n ≥ 2 the nth term is obtained by
 *            enumerating ternary strings of increasing length
 * Range:     u8 n <= 11, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0, high = ((idx_t)1 << 32) - 1;   /* high^2 fits 64‑bit */
    while (low <= high)
    {
        idx_t mid = low + (high - low) / 2;
        if (mid == 0 || mid <= n / mid)  /* mid*mid ≤ n  */
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;   /* floor(sqrt(n)) */
}

static seq_t pow3_u64(idx_t exp)
{
    seq_t res = 1;
    for (idx_t i = 0; i < exp; ++i)
        res *= 3;
    return res;
}

seq_t a(idx_t n)
{
    if (n == 1)               /* special first term */
        return (seq_t)0;

    /* offset of the requested term in the sequence that excludes the initial 0 */
    idx_t m = n - 1;

    /* find the smallest L ≥ 2 such that L(L-1)/2 ≥ m   (triangular numbers) */
    idx_t D = 1 + 8 * m;
    idx_t sqrtD = isqrt_u64(D);          /* floor(sqrt(D)) */
    idx_t L = (sqrtD + 1) / 2;            /* candidate length */
    if (L * (L - 1) / 2 < m)             /* adjust if needed */
        ++L;

    idx_t prev = (L - 2) * (L - 1) / 2;   /* terms before length L */
    idx_t offset = m - prev;              /* 1‑based position within this group (1 … L‑1) */

    /* zero position (0…L‑2) counted from the most significant digit side */
    idx_t pos = (L - 2) - (offset - 1);

    seq_t pow3L = pow3_u64(L);
    seq_t pow3pos = pow3_u64(pos);

    /* value = (3^L – 1)/2 – 3^pos */
    seq_t val = (pow3L - (seq_t)1) / 2 - pow3pos;
    return val;
}
