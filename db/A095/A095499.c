/*
 * A095499  a(n) = 53 written in base 13 - n.
 * https://oeis.org/A095499
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = representation of the decimal number 53 in base (13 - n),
 *            interpreted as a decimal integer
 * Range:     u8 n <= 8, u16 n <= 10, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function may be called with any non‑negative n; it returns 0 for n > 12
   because bases 13−n would be <= 0 for those indices. */
seq_t a(idx_t n)
{
    /* 13 − n will be the base for n = 0 … 12 (bases 13 down to 1). */
    if (n > 12)                /* undefined bases -> return 0. */
        return (seq_t)0;

    int base = 13 - (int)n;    /* base in [1,13] */

    /* Special case: base 1 – representation of a number is a string of
       that many ones. 53 ones → (10^53 − 1)/9.  We'll generate it by
       repeated multiplication and addition, which works for arbitrary
       precision seq_t. */
    if (base == 1) {
        seq_t r = 0;
        for (int i = 0; i < 53; ++i) {
            r = r * (seq_t)10 + (seq_t)1;
        }
        return r;
    }

    /* Compute the digits of 53 in the given base (least significant first). */
    int digits[64];            /* 53 in base 2 needs at most 6 digits; 53 in base 3 4 digits, etc. */
    int len = 0;
    seq_t num = (seq_t)53;

    while (num > 0) {
        int d = (int)(num % (seq_t)base);   /* remainder 0 … base-1 */
        digits[len++] = d;
        num = num / (seq_t)base;
    }

    /* Build the decimal integer by appending the digits from most significant
       to least significant. */
    seq_t result = 0;
    for (int i = len - 1; i >= 0; --i) {
        result = result * (seq_t)10 + (seq_t)digits[i];
    }

    return result;
}
