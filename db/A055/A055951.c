/*
 * A055951  n - reversal of base 5 digits of n (written in base 10).
 * https://oeis.org/A055951
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, sign
 * Formula:   a(n) = n - reverse_base5(n) where reverse_base5(n) is the
 *            integer obtained by writing n in base 5 and reversing those
 *            digits
 * Range:     i8 n <= 127, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function assumes that seq_t and idx_t are defined elsewhere.
   It relies only on arithmetic operations on seq_t. */
seq_t a(idx_t n) {
    seq_t rev = 0;          /* reversed base‑5 value */
    idx_t t   = n;
    while (t) {            /* extract base‑5 digits from least significant to most */
        int digit = t % 5;         /* digit is 0..4 */
        rev = rev * 5 + digit;     /* build reversed number in base 5 */
        t /= 5;
    }
    return (seq_t)n - rev; /* a(n) = n - reverse_base5(n) */
}
