/*
 * A204093  Numbers whose set of base-10 digits is {0,6}.
 * https://oeis.org/A204093
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = numbers whose decimal digits are only 0 or 6. a(0) = 0,
 *            a(1) = 6, a(2) = 60, a(3) = 66, …
 * Range:     u8 n <= 3, u16 n <= 23, u32 n <= 511, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th element of OEIS sequence A204093.
   Indexing starts at 0 (a(0) = 0). */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;

    /* m is the index among the numbers that start with 6 (excluding 0) */
    idx_t m = n;

    /* Find the minimal d such that 2^d ≥ m+1.
       This d equals the number of decimal digits of a(n) for n≥1. */
    idx_t d = 0;
    idx_t power = 1;            /* power == 2^d at loop end */
    idx_t temp = m + 1;
    while (temp > power) {
        power <<= 1;
        d++;
    }

    /* Count of numbers up to length d-1 (excluding 0). */
    idx_t prevCount = (power >> 1) - 1;   /* 2^(d-1) - 1 */
    idx_t position   = m - prevCount;     /* 1‑based index within block */
    idx_t suffix     = position - 1;      /* 0‑based binary suffix */
    idx_t numBits    = d - 1;             /* number of suffix digits */

    /* Construct the number: start with the leading 6. */
    seq_t result = (seq_t)6;
    for (idx_t i = numBits; i > 0; --i) {
        seq_t digit = ((suffix >> (i - 1)) & 1) ? (seq_t)6 : (seq_t)0;
        result = result * (seq_t)10 + digit;
    }

    return result;
}
