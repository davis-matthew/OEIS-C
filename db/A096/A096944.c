/*
 * A096944  Seventh column of (1,5)-Pascal triangle A096940.
 * https://oeis.org/A096944
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 13, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Validated: partial, pass, 36 terms through n = 35 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes C(n,5) for n >= 5 using multiplies and exact divides. */
static seq_t bin_choose5(idx_t n)
{
    seq_t r = 1;
    /* n*(n-1)*...*(n-4)/120 computed step by step. */
    for (idx_t i = 1; i <= 5; ++i)
    {
        r = r * (seq_t)(n - (5 - i)); /* numerator term */
        r = r / (idx_t)i;             /* divide by i, exact */
    }
    return r;
}
/*
 * Formula: a(n) = (n+30) * binomial(n+5,5) / 6
 * (Derived from a(n) = (n+30)*C(n+5,5)/6).
 */

seq_t a(idx_t n)
{
    /* C(n+5,5) */
    seq_t c = bin_choose5(n + 5);

    /* (n+30) * c / 6  – compute with 128‑bit intermediate if available. */
    seq_t mult = (seq_t)(n + 30);

#ifdef __SIZEOF_INT128__
    unsigned __int128 prod = (unsigned __int128)mult * (unsigned __int128)c;
    return (seq_t)(prod / 6);
#else
    /* Fallback: rely on built‑in multiplication (may overflow for very large n). */
    return (seq_t)((mult * c) / 6);
#endif
}
