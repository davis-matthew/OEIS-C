/*
 * A095625  a(n) = 117 written in base 10 - n.
 * https://oeis.org/A095625
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = 117 written in base (10 - n)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For n = 0..9 the base ranges from 10 down to 1, the decimal
   digits of that representation are concatenated to form a(n). */

seq_t a(idx_t n)
{
    /* Convert n to a signed integer to compute the base. */
    long long base = 10LL - (long long)n;

    /* Numbers are defined only for bases 1..10. For other bases
       (base <= 0) we return 0. */
    if (base <= 0) {
        return (seq_t)0;
    }

    /* Special case: base 1 (unary) representation of 117
       is the digit '1' repeated 117 times. */
    if (base == 1) {
        seq_t res = (seq_t)0;
        for (idx_t i = 0; i < (idx_t)117; ++i) {
            res = res * (seq_t)10 + (seq_t)1;
        }
        return res;
    }

    /* For bases 2..10 we perform a standard base conversion.
       The digits will all be between 0 and 9, so they can be
       concatenated directly. */
    seq_t num = (seq_t)117;
    seq_t digits[64];   /* 117 in any base requires at most 7 digits */
    int sz = 0;

    while (num > (seq_t)0) {
        seq_t rem = num % (seq_t)base;
        num = num / (seq_t)base;
        digits[sz++] = rem;
    }

    /* Build the result by concatenating the digits from most
       significant to least significant. */
    seq_t res = (seq_t)0;
    for (int i = sz - 1; i >= 0; --i) {
        res = res * (seq_t)10 + digits[i];
    }

    return res;
}
