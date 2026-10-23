/*
 * A035254  First differences of A035253.
 * https://oeis.org/A035254
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = A002024(n) + n - 3 A002024(n) is the sequence of numbers
 *            NOT of the form (k^2+5k)/2. The positive terms of a(n) are the
 *            complement of that pattern. This implementation constructs a(n)
 *            by counting how many forbidden values are ≤ x, and
 *            binary‑searching the smallest x such that complement_co...
 * Range:     i8 n <= 1, i16 n <= 28, i32 n <= 56, i64 n <= 56, i128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Integer square root: floor(sqrt(t)) for non‑negative integer t in seq_t. */
static seq_t isqrt_seq(seq_t t)
{
    seq_t low = 0, high = t;
    while (low <= high)
    {
        seq_t mid = low + (high - low) / 2;
        seq_t sq = mid * mid;
        if (sq <= t)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

/* Count how many forbidden numbers ≤ x.
   Forbidden numbers are (k^2+5k)/2 for k≥0. */
static seq_t forbidden_count(seq_t x)
{
    if (x < 0) return 0;
    /* Solve k^2 + 5k - 2x ≤ 0 → k ≤ (-5 + sqrt(25 + 8x)) / 2  (k ≥ 0). */
    seq_t v = (seq_t)25 + (seq_t)8 * x;
    seq_t s = isqrt_seq(v);
    seq_t k = 0;
    if (s >= 5)                          /* avoid underflow when s < 5 */
        k = (s - (seq_t)5) / 2;
    return k;
}

/* Number of allowed (positive) integers ≤ x. */
static seq_t allowed_count(seq_t x)
{
    if (x <= 0) return 0;
    return x - forbidden_count(x);
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Handle small negative terms explicitly. */
    if (n == 0) return (seq_t)-3;
    if (n == 1) return (seq_t)-1;

    /* For n ≥ 2, we need the (n-1)-th positive allowed number. */
    seq_t target = (seq_t)(n - 1);          /* required allowed count */

    /* Find an upper bound where the allowed count reaches target. */
    idx_t low  = 1;
    idx_t high = 1;
    while (true)
    {
        seq_t cnt = allowed_count((seq_t)high);
        if (cnt >= target) break;
        high = high * 2 + 1;            /* grow until we are big enough */
        if (high == UINT64_MAX)        /* safety for extreme values */
            break;
    }

    /* Binary search for the smallest x with allowed_count(x) ≥ target. */
    while (low < high)
    {
        idx_t mid = low + (high - low) / 2;
        seq_t cnt = allowed_count((seq_t)mid);
        if (cnt >= target)
            high = mid;
        else
            low = mid + 1;
    }
    return (seq_t)low;
}
