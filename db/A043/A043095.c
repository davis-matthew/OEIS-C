/*
 * A043095  Numbers with property that no two consecutive base 9 digits are
 *          equal.
 * https://oeis.org/A043095
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth integer ≥ 0 whose base‑9 representation * has no two
 *            consecutive equal digits
 * Range:     u8 n <= 212, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 9)          /* 0 … 8 */
        return (seq_t)(n - 1);

    /* find the length L of the base‑9 representation (digits count) */
    unsigned __int128 block = 64;   /* 8^2  (first block for length 2) */
    unsigned __int128 count = 9;    /* numbers counted so far (length 1) */
    idx_t L = 2;
    while ((unsigned __int128)n > count + block) {
        count += block;
        block *= 8;                 /* 8^L */
        ++L;
    }

    /* offset of the desired number within the block of length L
       (0‑based) */
    idx_t offset0 = n - count - 1;

    /* precompute powers of 8 up to L (used for position calculations) */
    unsigned __int128 pow8[64];
    pow8[0] = 1;
    for (idx_t i = 1; i <= L; ++i) pow8[i] = pow8[i - 1] * 8;

    /* first (most significant) digit */
    idx_t firstDigitIndex = offset0 / pow8[L - 1];  /* 0 … 7 */
    int firstDigit = (int)firstDigitIndex + 1;     /* 1 … 8 */
    unsigned __int128 idx = offset0 % pow8[L - 1];  /* remaining part */

    seq_t value = (seq_t)firstDigit;
    int prev = firstDigit;                 /* previous digit used */

    /* generate the remaining digits from most significant to least */
    int remaining = (int)(L - 1);
    for (int pos = remaining - 1; pos >= 0; --pos) {
        unsigned __int128 blockForDigit = pow8[pos];      /* 8^pos */
        idx_t digitIndex = (idx_t)(idx / blockForDigit);
        idx        = idx % blockForDigit;                /* remainder */

        /* choose the digit that is the digitIndex‑th smallest
           number in 0..8 excluding the previous digit */
        int digit = 0;
        int k = 0;
        for (int d = 0; d <= 8; ++d) {
            if (d == prev) continue;
            if (k == digitIndex) { digit = d; break; }
            ++k;
        }

        value = value * 9 + digit;  /* append digit to number */
        prev  = digit;              /* next digit cannot equal this one */
    }

    return value;
}
