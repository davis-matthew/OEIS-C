/*
 * A125144  Increments in the number of decimal digits of 4^n.
 * https://oeis.org/A125144
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = ⌊(n+1)·log10(4)⌋ − ⌊n·log10(4)⌋ * The value is 0 or 1,
 *            indicating whether the decimal digit count * increases when
 *            moving from 4ⁿ to 4ⁿ⁺¹
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 10¹² is a convenient scaling factor that fits in 64‑bit.
 * LOG4_SCALE stores ⌊10¹²·log10(4)⌋.
 * These constants are chosen so that
 *   0 ≤ n·log10(4) − (n·LOG4_SCALE) / 10¹² < 1/10¹²
 * for all n ≤ 100 000 (satisfying the constraints of the problem). */
static const unsigned long long SCALE      = 1000000000000ULL; /* 10¹² */
static const unsigned long long LOG4_SCALE = 602059991327ULL;  /* ⌊10¹²·log10(4)⌋ */

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1.  For n = 0 (below the offset)
     * we simply return 0; the exact value is irrelevant. */
    if (n == 0)
        return 0;

    /* Convert to 64‑bit to avoid overflow in the intermediate product.
     * The product fits in 64 bits for the domain of this problem
     * (n ≤ 10 000 → n·LOG4_SCALE ≤ 6·10¹⁵ < 2⁶³). */
    const unsigned long long n64 = (unsigned long long)n;

    unsigned long long d0 = (n64       * LOG4_SCALE) / SCALE;   /* ⌊n·log10(4)⌋  */
    unsigned long long d1 = ((n64 + 1) * LOG4_SCALE) / SCALE;   /* ⌊(n+1)·log10(4)⌋ */

    /* The difference is always 0 or 1.  Cast to seq_t for the return type. */
    return (seq_t)(d1 - d0);
}
