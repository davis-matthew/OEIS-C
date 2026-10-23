/*
 * A138223  a(n) = the nearest divisor of n to the number of positive divisors
 *          of n. In case of tie, round up.
 * https://oeis.org/A138223
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nearest divisor of n to the number of positive
 *            divisors of n; in a tie the larger divisor is chosen
 * Range:     u8 n <= 9063, u16 n <= 9063, u32 n <= 9063, u64 n <= 9063,
 *            u128 n <= 9063, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* integer square root for idx_t (unsigned 64‑bit) */
static idx_t _floor_sqrt_u64(idx_t n)
{
    if (n < 2) return n;
    idx_t low = 1, high = n >> 1;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        if (mid <= n / mid)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;    /* floor(sqrt(n)) */
}

/* number of positive divisors of n (τ(n)) */
static seq_t _tau_u64(idx_t n)
{
    seq_t tau = 0;
    idx_t limit = _floor_sqrt_u64(n);
    for (idx_t i = 1; i <= limit; ++i) {
        if (n % i == 0) {
            tau += 2;              /* i and n/i */
            if (i == n / i)        /* perfect square */
                tau -= 1;           /* count once */
        }
    }
    return tau;
}

/* sequence element a(n) for n >= 1 */
seq_t a(idx_t n)
{
    /* τ(n) */
    seq_t tau_seq = _tau_u64(n);
    idx_t tau = (idx_t)tau_seq;   /* τ(n) fits in idx_t for all 64‑bit n */

    idx_t best = 0;
    idx_t best_diff = (idx_t)-1;   /* very large initial value */

    idx_t limit = _floor_sqrt_u64(n);

    for (idx_t i = 1; i <= limit; ++i) {
        if (n % i) continue;

        idx_t d1 = i;
        idx_t diff1 = (d1 >= tau) ? d1 - tau : tau - d1;
        if (diff1 < best_diff || (diff1 == best_diff && d1 > best)) {
            best_diff = diff1;
            best = d1;
        }

        idx_t d2 = n / i;
        if (d2 != d1) {   /* avoid duplicate when n is a perfect square */
            idx_t diff2 = (d2 >= tau) ? d2 - tau : tau - d2;
            if (diff2 < best_diff || (diff2 == best_diff && d2 > best)) {
                best_diff = diff2;
                best = d2;
            }
        }
    }

    return (seq_t)best;
}
