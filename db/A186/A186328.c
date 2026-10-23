/*
 * A186328  Adjusted joint rank sequence of (f(i)) and (g(j)) with f(i) before
 *          g(j) when f(i)=g(j), where f and g are the pentagonal numbers and
 *          the hexagonal numbers. Complement of A186329.
 * https://oeis.org/A186328
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = rank of the nth pentagonal number in the sorted union of
 * pentagonal and hexagonal numbers, with ties resolved by ranking
 * the pentagonal number before the hexagonal number.
 * We compute it as a(n) = n + (number of hexagonal numbers < P(n)),
 * where P(n) = n*(3*n-1)/2.  The hexagonal numbers are H(k)=k*(2*k-1). */

#include <stdint.h>

static inline seq_t pentagonal(idx_t n)
{
    /* P(n) = n * (3*n - 1) / 2 */
    return ((seq_t)n) * (((seq_t)n * 3) - 1) / 2;
}

static inline seq_t hexagonal(idx_t k)
{
    /* H(k) = k * (2*k - 1) */
    return ((seq_t)k) * (((seq_t)2 * k) - 1);
}

/* Count hexagonal numbers strictly less than X using binary search.
 * X is the pentagonal value P(n). */
static inline idx_t count_hex_less_than(seq_t X)
{
    idx_t low = 0;
    idx_t high = 1;

    /* Increase high until it is >= the first hexagonal number >= X. */
    while (hexagonal(high) < X) {
        if (high > (UINT64_MAX >> 1))
            break;                /* avoid overflow; we are safe for n<=10000 */
        high <<= 1;
    }

    /* Binary search for smallest k such that H(k) >= X. */
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        if (hexagonal(mid) < X)
            low = mid + 1;
        else
            high = mid;
    }
    /* low is the first k with H(k) >= X, so count of H(k) < X is low-1. */
    return low - 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* the sequence is defined for n ≥ 1 */

    seq_t val = pentagonal(n);     /* pentagonal number P(n) */
    idx_t hex_count = count_hex_less_than(val);
    return (seq_t)n + (seq_t)hex_count;
}
