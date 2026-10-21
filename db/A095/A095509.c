/*
 * A095509  a(n) = 58 written in base 11 - n.
 * https://oeis.org/A095509
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A095509: a(n) is the decimal number obtained by representing
   the integer 58 in base (11 - n).  For n = 0..10 the bases are 11,10,9,…,
   2,1 respectively.  For base 1 the representation consists of 58 digits
   '1', i.e. the decimal number with 58 ones.  For n > 10 no base ≥ 1
   exists, so the function returns 0.  */
seq_t a(idx_t n)
{
    /* The binary value 58 is fixed.  */
    const int VALUE = 58;

    /* For n > 10 there is no valid base ≥ 1; return 0.  */
    if (n > 10)
        return (seq_t)0;

    /* Compute base  = 11 - n;  n ≤ 10, so cast to signed int is safe. */
    int base = 11 - (int)n;

    seq_t result = (seq_t)0;

    if (base >= 2) {               /* ordinary bases 2..11                         */
        int x = VALUE;
        /* Extract digits of VALUE in the given base.   */
        int digits[64];
        int len = 0;
        while (x > 0) {
            digits[len++] = x % base;
            x /= base;
        }

        /* Convert the digit sequence into a decimal integer.  */
        for (int i = len - 1; i >= 0; --i) {
            result = result * (seq_t)10 + (seq_t)digits[i];
        }
    } else if (base == 1) {        /* base‑1 representation: 58 copies of '1'    */
        for (int i = 0; i < VALUE; ++i) {
            result = result * (seq_t)10 + (seq_t)1;
        }
    } else {                       /* base <= 0: undefined for this sequence   */
        result = (seq_t)0;
    }

    return result;
}
