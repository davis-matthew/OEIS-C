/*
 * A061467  Remainder when the larger of n and its reverse is divided by the
 *          smaller.
 * https://oeis.org/A061467
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nice, nonn
 * Formula:   a(n) = larger of n and reverse(n) modulo smaller of n and
 *            reverse(n)
 * Range:     u8 n <= 207, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Special cases (palindromes, powers of 10, trailing‑zero numbers) automatically
   give remainder 0, so no separate handling is required. */

static inline idx_t rev_idx(idx_t n)
{
    idx_t rev = 0;
    while (n > 0) {
        rev = rev * 10 + (n % 10);
        n   = n / 10;
    }
    return rev;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* a(0)=0 */

    idx_t r = rev_idx(n);

    /* larger and smaller as seq_t */
    seq_t larger, smaller;
    if (r > n) {
        larger  = (seq_t)r;
        smaller = (seq_t)n;
    } else {
        larger  = (seq_t)n;
        smaller = (seq_t)r;
    }

    /* division by zero cannot happen here (smallest>0 except n==0 already handled) */
    return larger % smaller;
}
