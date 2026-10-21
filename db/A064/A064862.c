/*
 * A064862  Period of continued fraction for sqrt(10)*n.
 * https://oeis.org/A064862
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = period of the continued fraction for sqrt(10)*n
 *           using the standard continued–fraction algorithm.
 *
 * The period of sqrt(D) for a non‑square D ends when the
 * partial quotient equals 2*a0, where a0 = ⌊√D⌋.  For D = 10*n²
 * this condition is always reached.
 */

#include <stdint.h>

/* ---------- auxiliary integer square root ---------- */
/* Computes ⌊√x⌋ for an arbitrary‑precision seq_t value x. */
static seq_t isqrt_seq(seq_t x)
{
    seq_t low = 0;
    seq_t high = x;
    while (low < high)
    {
        seq_t mid = (low + high + 1) / 2;          /* midpoint, upper‑biased */
        if (mid * mid <= x)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    /* D = 10 * n^2  (always non‑square for integer n > 0) */
    seq_t D = (seq_t)10 * (seq_t)n * (seq_t)n;

    /* a0 = ⌊√D⌋ */
    seq_t a0 = isqrt_seq(D);

    seq_t m   = 0;
    seq_t d   = 1;
    seq_t a   = a0;   /* current partial quotient */
    seq_t period = 0;

    for (;;)
    {
        m = d * a - m;
        d = (D - m * m) / d;
        a = (a0 + m) / d;
        period++;

        /* period ends when a == 2*a0 */
        if (a == a0 * 2)
            break;
    }

    return period;
}
