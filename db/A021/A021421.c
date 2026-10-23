/*
 * A021421  Decimal expansion of 1/417.
 * https://oeis.org/A021421
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A021421:  Decimal expansion of 1/417.
   The sequence is periodic with period 46.
   a(0) = 0 (the integer part);
   for n ≥ 1, a(n) = d((n+1) mod 46), where d(k) is the k‑th
   decimal digit of 1/417.  For n > 45, a(n) = a(n‑46). */

seq_t a(idx_t n)
{
    static seq_t digits[46] = {0};
    static int initialized = 0;

    if (!initialized) {
        uint64_t rem   = 1ULL;   /* remainder after integer part */
        const uint64_t denom = 417ULL;
        for (idx_t i = 0; i < 46; ++i) {
            uint64_t prod  = rem * 10ULL;
            uint64_t digit = prod / denom;
            rem            = prod % denom;
            digits[i] = (seq_t)digit;
        }
        initialized = 1;
    }

    /* Period length */
    const idx_t period = 46ULL;

    /* Compute index in [1..46] corresponding to (n+1) mod 46,
       but treating 0 as 46. */
    idx_t idx = (n + 1ULL) % period;
    if (idx == 0) idx = period;

    return digits[idx - 1U];
}
