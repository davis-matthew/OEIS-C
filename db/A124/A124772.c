/*
 * A124772  Number of set partitions associated with compositions in standard
 *          order.
 * https://oeis.org/A124772
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, look, tabf
 * Formula:   a(n) = Product_{i=1}^k C((sum_{j=i}^k b(j))-1, b(i)-1) for the
 *            nth composition in standard order (A066099)
 * Range:     u8 n <= 273, u16 n <= 4244, u32 n <= 8750, u64 n <= 8750,
 *            u128 n <= 8750, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* --------------------------------------------------------------------- */
#include <stdint.h>

static seq_t seq_gcd(seq_t a, seq_t b)
{
    while (b)
    {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

static seq_t binom_small(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i)
    {
        seq_t num = (seq_t)(n - k + i);  /* numerator part   */
        seq_t den = (seq_t)i;            /* denominator part */
        seq_t g1 = seq_gcd(num, den);
        num /= g1;
        den /= g1;
        seq_t g2 = seq_gcd(res, den);
        res /= g2;
        den /= g2;
        res = res * num;
        res = res / den;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Handle empty composition: a(0) = 1 */
    if (n == 0) return (seq_t)1;

    /* Determine the sum (total of parts) for the nth composition.
       Cumulative counts: cum(s) = 2^s, with 2^0 = 1 for the empty composition. */
    idx_t s = 1;
    while (((idx_t)1 << s) <= n) ++s;          /* smallest s with 2^s > n */
    idx_t offset = n - (((idx_t)1 << (s - 1))); /* 0‑based index within part of sum s */

    /* Build the part sizes from the binary representation of offset.
       Bits are interpreted from most significant to least. */
    idx_t parts[64];           /* s≤64 for all practical n (n up to ~10^6). */
    idx_t partCnt = 0;
    idx_t partLen = 1;

    for (idx_t i = 1; i <= s - 1; ++i)
    {
        idx_t bitPos = s - 1 - i;                /* MSB corresponds to cut after element i */
        if ((offset >> bitPos) & 1ULL)           /* cut after this element */
        {
            parts[partCnt++] = partLen;
            partLen = 1;
        }
        else
            ++partLen;
    }
    parts[partCnt++] = partLen;                  /* final part */

    /* Compute the product of binomial coefficients according to the formula. */
    seq_t result = (seq_t)1;
    idx_t remaining = s;

    for (idx_t i = 0; i < partCnt; ++i)
    {
        idx_t len = parts[i];
        result *= binom_small(remaining - 1, len - 1);
        remaining -= len;
    }
    return result;
}
