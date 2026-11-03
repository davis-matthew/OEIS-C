/*
 * A108712  A fractal sequence, defined by a(2n-1) = A007376(n) (the
 *          almost-natural numbers), a(2n) = a(n).
 * https://oeis.org/A108712
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(2n-1) = A007376(n); a(2n) = a(n)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Validated: partial, pass, 105 terms through n = 105 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t A007376(idx_t k)
{
    /* first 10 digits are 0‑9 */
    if (k < 10)
        return (seq_t)k;

    idx_t pos      = k - 10;           /* offset beyond the first 10 digits */
    idx_t len      = 2;                /* current length of numbers in the block */
    idx_t ten_pow  = 10;               /* 10^(len-1), first number with 'len' digits */
    idx_t start    = ten_pow;          /* first number of this length */

    /* Find the block containing the k‑th digit */
    while (1) {
        idx_t numbers = 9 * ten_pow;     /* count of numbers with 'len' digits */
        idx_t digits  = numbers * len;    /* total digits contributed by this block */
        if (pos < digits)
            break;

        pos   -= digits;
        len   += 1;
        ten_pow *= 10;
        start = ten_pow;
    }

    /* Within the block: locate the exact number and digit */
    idx_t num_index   = pos / len;
    idx_t digit_index = pos % len;      /* 0 = most significant digit */

    idx_t number = start + num_index;

    /* Compute the digit: divide by the appropriate power of 10 */
    idx_t divisor = 1;
    for (idx_t i = 0; i < len - 1 - digit_index; ++i)
        divisor *= 10;

    seq_t digit = (seq_t)((number / divisor) % 10);
    return digit;
}

seq_t a(idx_t n)
{
    /* Reduce even indices by dividing by 2 until the index becomes odd.
     * For an odd index n, we have n = 2k-1, so a(n) = A007376(k). */
    while ((n & 1) == 0) {
        n >>= 1;          /* n /= 2 */
    }
    return A007376((n + 1) / 2);
}
