/*
 * A214302  Expansion of f(-x^2, -x^4) * f(x^3, x^5) in powers of x where f(,)
 *          is Ramanujan's two-variable theta function.
 * https://oeis.org/A214302
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of x^n in (Sum_{k∈Z} (-1)^k x^(3k^2 + k)) ·
 *            (Sum_{k∈Z} x^(4k^2 + k)) Implemented by direct summation over
 *            all integer k such that 3k^2 + k ≤ n. The second series
 *            contributes 1 iff the residue n - (3k^2 + k) equals 4j^2 + j for
 *            some integer j (positive, zero, or negative). A sma...
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* ---------- integer square root (floor) ---------- */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t x = 0, bit = ((idx_t)1) << 62;          /* highest power of 2 ≤ 2^63 */
    while (bit)
    {
        idx_t nxt = x + bit;
        if ((unsigned __int128)nxt * nxt <= (unsigned __int128)n)
            x = nxt;
        bit >>= 1;
    }
    return x;
}

/* ---------- test if m can be written as 4j^2 + j (any integer j) ---------- */
static inline bool is_second_exponent(idx_t m)
{
    /*  m = 4j^2 + j   ⇔   (2*2j+1)^2 = 1 + 16m   */
    idx_t D = 1 + 16 * m;
    idx_t r = isqrt_u64(D);
    if ((unsigned __int128)r * r != (unsigned __int128)D)
        return false;
    /*  r ≡ 1 (mod 8)  →  j = (r‑1)/8  (non‑negative j)   */
    /*  r ≡ 7 (mod 8)  →  j = –(r‑1)/8  (negative j)       */
    return (r & 7) == 1 || (r & 7) == 7;
}

/* ---------- sequence generator ---------- */
seq_t a(idx_t n)
{
    seq_t sum = 0;

    /* find maximum positive k such that 3k^2 + k ≤ n */
    int64_t maxKpos = -1;
    for (int64_t k = 0;; ++k)
    {
        int64_t e1 = 3 * k * k + k;
        if (e1 > (int64_t)n) { maxKpos = k - 1; break; }
    }

    /* find maximum |k| (negative side) such that 3k^2 - |k| ≤ n */
    int64_t maxKneg = -1;
    for (int64_t t = 0;; ++t)
    {
        int64_t e1 = 3 * t * t - t;      /* for k = –t */
        if (e1 > (int64_t)n) { maxKneg = t - 1; break; }
    }

    for (int64_t k = -maxKneg; k <= maxKpos; ++k)
    {
        int64_t e1 = 3 * k * k + k;        /* 3k^2 + k */
        if (e1 > (int64_t)n) continue;
        int64_t e2 = (int64_t)n - e1;      /* residue to be checked */
        if (e2 < 0) continue;
        if (is_second_exponent((idx_t)e2))
        {
            if (k & 1) sum -= 1;
            else      sum += 1;
        }
    }
    return sum;
}
