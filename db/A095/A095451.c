/*
 * A095451  a(n) = 29 written in base 14 - n.
 * https://oeis.org/A095451
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = the base‑(14‑n) representation of 29, read as a decimal
 *            number. For 0 ≤ n ≤ 13 this gives the terms 21, 23, 25, …,
 *            11101, and for n = 13 the unary representation 111…1 (29 ones).
 *            For n > 13 the sequence is undefined; this implementation
 *            returns 0
 * Range:     u8 n <= 10, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function must have external linkage. */
seq_t a(idx_t n)
{
    /* Only defined for n ≤ 13 (base 1 – unary).  Return 0 otherwise. */
    if (n > (idx_t)13) {
        return (seq_t)0;
    }

    /* Base of the representation. */
    idx_t base = (idx_t)14 - n;

    /* Unary base 1: 29 ones.  Build the decimal number directly. */
    if (base == (idx_t)1) {
        seq_t result = 0;
        for (idx_t i = 0; i < (idx_t)29; ++i) {
            result = result * (seq_t)10 + (seq_t)1;
        }
        return result;
    }

    /* For bases 2 … 14.  Convert 29 into that base and build the decimal
       number from the most significant digit. */
    seq_t value = (seq_t)29;
    int digits[30];          /* plenty of space for all possibilities */
    int len = 0;

    /* Extraction of digits (least significant first). */
    while (value > (seq_t)0) {
        digits[len++] = (int)(value % (seq_t)base);
        value = value / (seq_t)base;
    }

    /* Assemble the decimal number from most significant digit. */
    seq_t result = 0;
    for (int i = len - 1; i >= 0; --i) {
        result = result * (seq_t)10 + (seq_t)digits[i];
    }
    return result;
}
