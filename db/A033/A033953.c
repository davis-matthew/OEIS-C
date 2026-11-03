/*
 * A033953  Write 0,1,2,... in a clockwise spiral on a square lattice, writing
 *          each digit at a separate lattice point, starting with 0 at the
 *          origin and 1 at x=0, y=-1; sequence gives the numbers on the
 *          positive x-axis.
 * https://oeis.org/A033953
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, nice
 * Formula:   a(n) = A033307(4*n^2 + 3*n) for n>0, a(0)=0
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Validated: partial, pass, 105 terms through n = 104 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Return the k‑th (0‑based) digit of the infinite string
   formed by concatenating 0,1,2,3,....  */
static seq_t digit_at_index(idx_t k)
{
    /* First 10 digits are trivial */
    if (k < 10)
        return (seq_t)k;

    idx_t remaining = k - 10;   /* Skip the 0‑9 block */
    idx_t pow10   = 10;          /* 10^(d-1) for d=2 */
    int    d      = 2;           /* current digit length */

    for (;; ++d) {
        idx_t count   = 9 * pow10;             /* numbers in this block */
        idx_t digits  = count * (idx_t)d;       /* digits contributed */

        if (remaining < digits)
            break;                            /* target lies here */

        remaining -= digits;
        pow10 *= 10;                          /* move to next block */
    }

    /* number_offset and digit_offset are 0‑based */
    idx_t number_offset = remaining / (idx_t)d;
    idx_t digit_offset  = remaining % (idx_t)d;
    idx_t number        = pow10 + number_offset;

    /* Place value of the desired digit within the number */
    idx_t place = 1;
    int   exp   = d - 1 - (int)digit_offset;
    for (int i = 0; i < exp; ++i)
        place *= 10;

    idx_t digit = (number / place) % 10;
    return (seq_t)digit;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* Compute index in the digit stream */
    idx_t idx = 4 * n * n + 3 * n;   /* 4*n^2 + 3*n, fits in idx_t for n up to 10^9 */
    return digit_at_index(idx);
}
