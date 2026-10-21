/*
 * A098734  Consider the sequence {b(n), n >= 1} of digits of the integers: 0
 *          1 2 3 4 5 6 7 8 9 1 0 1 1 1 2 1 3 1 4 1 5 1 6 1 7 1 8 1 9 2 0...
 *          (A033307); a(n) = n*b(n).
 * https://oeis.org/A098734
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = n * b(n), where b(n) is the nth digit of the
 *            concatenation of non‑negative integers (0,1,2,3,…)
 * Range:     u8 n <= 29, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static idx_t pow10_idx(idx_t e) {
    idx_t r = 1;
    while (e--) r *= 10;
    return r;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* offset is 1, but guard against 0 */

    idx_t idx = n - 1;                     /* 0‑based position in the digit stream */
    idx_t len = 1;
    idx_t start = 0;
    idx_t count, digitsInGroup;

    while (1) {
        if (len == 1)
            count = 10;                /* digits 0–9 */
        else
            count = 9 * pow10_idx(len - 1); /* numbers with exactly len digits */

        digitsInGroup = count * len;
        if (idx < digitsInGroup)
            break;

        idx -= digitsInGroup;
        len++;
        start = pow10_idx(len - 1);      /* first number with this length */
    }

    idx_t numberIndex = idx / len;
    idx_t pos   = idx % len;             /* 0 = most significant digit of the number */

    idx_t num = start + numberIndex;     /* the actual integer containing the digit */

    /* extract the digit at position 'pos' (most significant first) */
    idx_t pow = pow10_idx(len - pos - 1);
    seq_t digit = (seq_t)((num / pow) % 10);

    return (seq_t)n * digit;
}
