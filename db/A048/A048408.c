/*
 * A048408  Numbers whose consecutive digits differ by 6.
 * https://oeis.org/A048408
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 34, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers whose consecutive digits differ by 6.
   The terms are enumerated in increasing order.
   For n < 10 the term is n itself.
   For n >= 10 the term has length L = (n-10)/7 + 2
   and starts with one of the digits {1,2,3,6,7,8,9}.
   The starting digit is chosen according to the offset
   within the block of seven numbers of that length; the digits
   then alternate between the starting digit and its partner
   (0 for 6, 1/7, 2/8 and 3/9). */

seq_t a(idx_t n)
{
    if (n < 10) return (seq_t)n;

    idx_t k = n - 10;          /* position after the initial 10 terms */
    idx_t block = k / 7;       /* number of complete length‑blocks */
    idx_t offset = k % 7;      /* offset within the current block */
    idx_t length = block + 2;  /* length of the resulting number */

    /* ordered list of possible leading digits for lengths >= 2 */
    static const int starts[7] = {1, 2, 3, 6, 7, 8, 9};
    int start_digit = starts[offset];

    /* partner digits for each starting digit */
    int partner;
    switch (start_digit) {
        case 1: partner = 7; break;
        case 7: partner = 1; break;
        case 2: partner = 8; break;
        case 8: partner = 2; break;
        case 3: partner = 9; break;
        case 9: partner = 3; break;
        case 6: partner = 0; break;
        default: partner = 0; /* unreachable */
    }

    seq_t value = 0;
    for (idx_t i = 0; i < length; ++i) {
        int digit = (i % 2 == 0) ? start_digit : partner;
        value = value * (seq_t)10 + (seq_t)digit;
    }
    return value;
}
