/*
 * A040839  Continued fraction for sqrt(869).
 * https://oeis.org/A040839
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction for sqrt(869).
   a(0) = ⌊√869⌋ = 29.
   For n ≥ 1 the terms repeat with a period found by the
   standard square‑root continued fraction algorithm. */
#include <stddef.h>
#include <stdint.h>

/* Integer square root of a 64‑bit unsigned number.
   Return ⌊√n⌋.  Division is used to avoid overflow. */
static seq_t _isqrt_u64(uint64_t n)
{
    uint64_t lo = 0, hi = n;
    while (lo <= hi)
    {
        uint64_t mid = lo + ((hi - lo) >> 1);
        if (mid == 0) { lo = 1; continue; }
        if (mid > n / mid)
            hi = mid - 1;
        else
            lo = mid + 1;
    }
    return (seq_t)hi;
}

seq_t a(idx_t n)
{
    static seq_t period[128];
    static size_t period_len = 0;
    static int initialized = 0;
    static seq_t a0;          /* floor(sqrt(869)) = 29 */

    if (!initialized)
    {
        const idx_t D = 869;
        a0 = _isqrt_u64(D);
        const idx_t a0_idx = (idx_t)a0;

        idx_t m = 0;
        idx_t d = 1;
        idx_t a_prev = a0_idx;
        idx_t a_next = 0;

        /* Generate the period until the terminating term 2*a0 appears */
        do
        {
            m = d * a_prev - m;
            d = (D - m * m) / d;
            a_next = (a0_idx + m) / d;
            period[period_len++] = (seq_t)a_next;
            a_prev = a_next;
        } while (a_next != 2 * a0_idx);

        initialized = 1;
    }

    if (n == 0)
        return a0;

    const idx_t idx = (n - 1) % (idx_t)period_len;
    return period[(size_t)idx];
}
