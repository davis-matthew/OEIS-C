/*
 * A204057  Triangle derived from an array of f(x), Narayana polynomials.
 * https://oeis.org/A204057
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is obtained by reading the infinite triangle whose entry in
 *            row i (i≥1) and column j (j≥1) is the Narayana polynomial G_j
 *            evaluated at x = i−1: T(i,j) = G_j(i−1) = Σ_{k=1}^{j} N(j,k) ·
 *            (i−1)^{k−1}, where N(j,k) = (1/j)·C(j,k)·C(j,k−1) are the
 *            Narayana numbers. The sequence a(n) lists the ele...
 * Range:     u8 n <= 32, u16 n <= 62, u32 n <= 147, u64 n <= 369,
 *            u128 n <= 977, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#undef  _floor_isqrt_u64 /* ensure we use our own portable sqrt */
static inline idx_t
isqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if (mid <= n / mid)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* Forward declarations of helper functions supplied by the build.
   They must be available from "../utils/...". */
#include "../utils/binom.h"

#ifndef __cplusplus
/* 64‑bit C implementation: use a tight loop with overflow checks. */
seq_t a(idx_t n)
{
    /* Find the antidiagonal. */
    idx_t disc = 1 + 8 * n;
    idx_t sqrt_disc = isqrt_u64(disc);
    idx_t s = (1 + sqrt_disc) >> 1;                     /* (1+√disc)/2  */
    /* Adjust s if necessary. */
    if ((unsigned __int128)s * (s - 1) / 2 < (unsigned __int128)n) ++s;

    /* Compute preceding elements count and offsets. */
    unsigned __int128 t_prev128 = (unsigned __int128)(s - 1) * (s - 2) / 2;
    idx_t t_prev = (idx_t)t_prev128;                     /* fits in idx_t */
    idx_t offset = n - t_prev - 1;
    idx_t i = s - 1 - offset;                           /* row index  */
    idx_t j = offset + 1;                               /* column index */

    /* Prepare for computing T(i,j). */
    unsigned __int128 base = (unsigned __int128)(i > 0 ? i - 1 : 0);
    unsigned __int128 power = 1;        /* base^0 */
    unsigned __int128 res   = 0;
    const unsigned __int128 limit = (unsigned __int128)((seq_t)-1); /* max seq_t */

    for (idx_t k = 1; k <= j; ++k) {
        seq_t binom_jk      = binom(j, k);
        seq_t binom_jk_minus = binom(j, k - 1);
        unsigned __int128 bin1 = (unsigned __int128)binom_jk;
        unsigned __int128 bin2 = (unsigned __int128)binom_jk_minus;

        if (bin1 > limit || bin2 > limit)    /* term will overflow */
            continue;

        unsigned __int128 product = bin1 * bin2;
        unsigned __int128 termCoef = product / j;   /* N(j,k) */

        if (termCoef > limit)
            continue;

        /* Check multiplication overflow for term. */
        if (termCoef != 0 && power > limit / termCoef)
            break;        /* all subsequent terms would overflow */

        unsigned __int128 term = termCoef * power;
        if (res > limit - term)
            break;        /* sum would overflow */

        res += term;

        /* Update power for next exponent if not last iteration. */
        if (k < j) {
            if (base == 0) {
                power = 0;
            } else if (power > limit / base) {
                power = limit + 1;   /* overflow indicator */
            } else {
                power *= base;
            }
        }
    }
    return (seq_t)res;
}

#else /* C++17 implementation */
#include <limits>
#include <type_traits>

/* If seq_t is an integral type with a specialization of std::numeric_limits
   (i.e., uint8_t … uint64_t), we use the overflow‑sensitive algorithm above.
   For larger or arbitrary‑precision seq_t types we compute the full value. */
template <typename T>
constexpr bool small_seq_t =
    std::is_integral<T>::value &&
    std::numeric_limits<T>::is_specialized &&
    (sizeof(T) <= sizeof(uint64_t));

template <typename T>
constexpr bool large_seq_t = !small_seq_t<T>;

static inline idx_t
isqrt_u64_cpp(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if (mid <= n / mid)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* Compute antidiagonal parameters. */
    idx_t disc = 1 + 8 * n;
    idx_t sqrt_disc = isqrt_u64_cpp(disc);
    idx_t s = (1 + sqrt_disc) >> 1;
    if ((unsigned __int128)s * (s - 1) / 2 < (unsigned __int128)n)
        ++s;
    unsigned __int128 t_prev128 = (unsigned __int128)(s - 1) * (s - 2) / 2;
    idx_t t_prev = (idx_t)t_prev128;
    idx_t offset = n - t_prev - 1;
    idx_t i = s - 1 - offset;
    idx_t j = offset + 1;

    if constexpr (small_seq_t<seq_t>) {
        /* Small seq_t (≤64‑bit) implementation. */
        unsigned __int128 base = (unsigned __int128)(i > 0 ? i - 1 : 0);
        unsigned __int128 power = 1;
        unsigned __int128 res   = 0;
        const unsigned __int128 limit = (unsigned __int128)((seq_t)-1);

        for (idx_t k = 1; k <= j; ++k) {
            seq_t binom_jk      = binom(j, k);
            seq_t binom_jk_minus = binom(j, k - 1);
            unsigned __int128 bin1 = (unsigned __int128)binom_jk;
            unsigned __int128 bin2 = (unsigned __int128)binom_jk_minus;

            if (bin1 > limit || bin2 > limit)
                continue;

            unsigned __int128 product = bin1 * bin2;
            unsigned __int128 termCoef = product / j;

            if (termCoef > limit)
                continue;

            if (termCoef != 0 && power > limit / termCoef)
                break;

            unsigned __int128 term = termCoef * power;
            if (res > limit - term)
                break;

            res += term;

            if (k < j) {
                if (base == 0)
                    power = 0;
                else if (power > limit / base)
                    power = limit + 1;
                else
                    power *= base;
            }
        }
        return (seq_t)res;
    } else {
        /* Large or arbitrary‑precision seq_t implementation. */
        seq_t base = (i > 0 ? (seq_t)(i - 1) : (seq_t)0);
        seq_t power = 1;           /* base^0 */
        seq_t res   = 0;

        for (idx_t k = 1; k <= j; ++k) {
            seq_t binom_jk      = binom(j, k);
            seq_t binom_jk_minus = binom(j, k - 1);
            seq_t product = binom_jk * binom_jk_minus;   /* exact */
            seq_t termCoef = product / (seq_t)j;         /* integer division */
            seq_t term = termCoef * power;
            res = res + term;

            if (k < j)
                power = power * base;
        }
        return res;
    }
}
#endif
