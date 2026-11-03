/*
 * A108202  a(2n) = A007376(n); a(2n+1) = a(n).
 * https://oeis.org/A108202
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   * a(0) = 0 * a(2*n) = A007376(n) // digit of natural numbers
 *            written in order * a(2*n+1) = a(n) * * Small helper: A007376(k)
 *            returns the k‑th digit (0‑indexed) of *
 *            0,1,2,3,4,5,6,7,8,9,10,11,
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Validated: partial, pass, 105 terms through n = 104 (2026-11-01)
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
    /* Reduce odd indices by repeatedly dividing by 2 until even (or zero). */
    while (n & 1)
        n >>= 1;

    /* For even n (or zero) return the digit at position n/2. */
    return A007376(n / 2);
}
