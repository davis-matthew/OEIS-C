/*
 * A048337  a(n) in base 13 is a repdigit.
 * https://oeis.org/A048337
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (n - 12*⌊(n-1)/12⌋)*(13^⌊(n+11)/12⌋ - 1)/12
 * Range:     u8 n <= 25, u16 n <= 50, u32 n <= 100, u64 n <= 205,
 *            u128 n <= 412, big any n
 * Validated: standard, pass, 67 terms through n = 1178 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the 13‑power sum 1 + 13 + 13² + … + 13^(k‑1) without
 * overflowing for 64‑bit seq_t values.  The loop keeps the intermediate
 * power below 13^(k‑1).  This is safe because the final result fits in
 * seq_t whenever the returned a(n) does. */
static inline seq_t repdigit_sum(idx_t k)
{
    seq_t power = (seq_t)1;   /* 13^i */
    seq_t sum   = (seq_t)0;
    for (idx_t i = 0; i < k; ++i) {
        sum += power;
        /* Do not compute 13^k once the last term has been added. */
        if (i + 1 == k) break;
        power *= (seq_t)13;
    }
    return sum;
}

seq_t a(idx_t n)
{
    /* Handle the offset.  The definition in the OEIS is a(0)=0. */
    if (n == 0) return (seq_t)0;

    /* Compute floor((n-1)/12) and floor((n+11)/12) for unsigned n. */
    idx_t lower = (n - 1) / 12;
    idx_t k     = (n + 11) / 12;

    /* Digit d = n - 12*floor((n-1)/12), 1 <= d <= 12 for n>0. */
    idx_t d = n - 12 * lower;

    /* The repdigit multiplier: (13^k - 1) / 12 = 1 + 13 + … + 13^(k-1). */
    seq_t rep = repdigit_sum(k);

    /* Final result.  Multiplication by d (≤12) cannot overflow when
     * a(n) fits in seq_t. */
    return ((seq_t)d) * rep;
}
