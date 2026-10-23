/*
 * A171091  Digits in the order in which they appear in the fractional part of
 *          the decimal expansion of Pi.
 * https://oeis.org/A171091
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, fini, full, less
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is the order of first appearance of digits 0‑9 in the fractional
   part of Pi.  It is obtained by scanning the digits of Pi and returning the
   nth digit that first appears. */
seq_t a(idx_t n) {
    if (n == 0) return 0; /* offset starts at 1 */
    static const char pi_digits[] =
        "14159265358979323846264338327950288419716939937510"
        "58209749445923078164062862089986280348253421170679"
        "82148086513282306647093844609550582231725359408128"
        "48111745028410270193852110555964462294895493038196"
        "44288109756659334461284756482337867831652712019091"
        "45648566923460348610454326649"; /* 200+ digits of Pi */
    unsigned char seen[10] = {0};
    unsigned char count = 0;
    idx_t i;
    for (i = 0; pi_digits[i] != '\0'; ++i) {
        unsigned char d = (unsigned char)(pi_digits[i] - '0');
        if (!seen[d]) {
            seen[d] = 1;
            ++count;
            if (count == n) {
                return (seq_t)d;
            }
        }
    }
    return 0; /* n exceeds number of distinct digits (should not happen) */
}
