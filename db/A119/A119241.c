/*
 * A119241  Number of powerful numbers (A001694) between consecutive squares
 *          n^2 and (n+1)^2.
 * https://oeis.org/A119241
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = F((n+1)^2 - 1) - F(n^2), where F(x) counts powerful
 *            numbers ≤ x. A powerful number is one whose every prime divisor
 *            occurs with exponent ≥ 2. Every such number can be written
 *            uniquely as a²·b³ with b square‑free. Hence F(x) = Σ_{b
 *            square‑free, b³ ≤ x} ⌊ √(x / b³) ⌋
 * Range:     u8 n <= 13, u16 n <= 1893, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/* ---------- helpers for square‑free numbers ---------- */
static uint64_t *squarefree = NULL;
static size_t sf_len = 0;

/* Build a list of all square‑free numbers up to some handy limit.
   600 is enough for n ≤ 10 000, because for such n
   (n+1)²−1 ≤ 1 000 20000 and ∛X ≤ 464. */
static void init_squarefree(void)
{
    if (squarefree != NULL)
        return;                     /* already built */

    const uint64_t MAX_B = 600;      /* safe upper bound */
    squarefree = (uint64_t*)malloc(MAX_B * sizeof(uint64_t));
    uint8_t *is_sqf = (uint8_t*)malloc((MAX_B + 1) * sizeof(uint8_t));
    if (!squarefree || !is_sqf)
        return;                     /* allocation failed – unlikely */

    /* initially assume every number is square‑free */
    for (uint64_t i = 1; i <= MAX_B; ++i)
        is_sqf[i] = 1;

    /* eliminate numbers that contain a square prime factor */
    for (uint64_t p = 2; p * p <= MAX_B; ++p)
    {
        if (is_sqf[p])
        {
            uint64_t sq = p * p;
            for (uint64_t m = sq; m <= MAX_B; m += sq)
                is_sqf[m] = 0;
        }
    }

    /* collect all square‑free numbers into the array */
    sf_len = 0;
    for (uint64_t i = 1; i <= MAX_B; ++i)
        if (is_sqf[i])
            squarefree[sf_len++] = i;

    free(is_sqf);
}

/* ---------- integer square root ---------- */
/* Assumes the argument fits into 64 bits. */
static inline seq_t isqrt_seq(seq_t x)
{
    uint64_t v = (uint64_t)x;
    uint64_t lo = 0, hi = v;

    while (lo <= hi)
    {
        uint64_t mid = lo + ((hi - lo) >> 1);
        uint64_t sq  = mid * mid;

        if (sq == v)
            return (seq_t)mid;
        if (sq < v)
            lo = mid + 1;
        else
        {
            if (mid == 0)
                break;
            hi = mid - 1;
        }
    }
    return (seq_t)hi;
}

/* ---------- count powerful numbers ≤ X ---------- */
static seq_t count_powerful_upto(seq_t X)
{
    init_squarefree();

    seq_t count = 0;
    for (size_t i = 0; i < sf_len; ++i)
    {
        uint64_t b  = squarefree[i];
        seq_t  b3   = (seq_t)b * (seq_t)b * (seq_t)b;
        if (b3 > X)
            break;

        seq_t  t    = X / b3;          /* floor division */
        seq_t  sqt  = isqrt_seq(t);    /* floor sqrt of t */
        count += sqt;
    }
    return count;
}

/* ---------- the required sequence function ---------- */
seq_t a(idx_t n)
{
    seq_t n1   = (seq_t)n;
    seq_t x0   = n1 * n1;
    seq_t x1   = (n1 + 1) * (n1 + 1) - 1;

    seq_t c0   = count_powerful_upto(x0);
    seq_t c1   = count_powerful_upto(x1);

    return c1 - c0;
}
