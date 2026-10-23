/*
 * A033619  Undulating numbers (of form abababab... in base 10).
 * https://oeis.org/A033619
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n‑th integer whose decimal digits alternate between
 *            two * digits a and b (with a ≠ 0). The numbers are listed in
 *            increasing order
 * Range:     u8 n <= 116, u16 n <= 335, u32 n <= 763, u64 n <= 1639,
 *            u128 n <= 3364, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= (idx_t)10) {
        return (seq_t)(n - (idx_t)1);
    }

    /* Numbers with length ≥ 2 are grouped in blocks of 90:
     * 9 choices for a (1–9) and 10 choices for b (0–9).
     * The order within a block is a first, then b. */
    idx_t offset   = n - (idx_t)10;      /* 1‑based offset from the start of 2‑digit numbers */
    idx_t idx0     = offset - (idx_t)1;  /* 0‑based index within that part */
    idx_t group    = idx0 / (idx_t)90;   /* which length group (0 → L = 2) */
    idx_t length   = group + (idx_t)2;   /* actual digit length */
    idx_t posWithin = idx0 % (idx_t)90;  /* position inside the group */

    int a_digit = 1 + (int)(posWithin / (idx_t)10); /* a in 1..9 */
    int b_digit = (int)(posWithin % (idx_t)10);     /* b in 0..9 */

    seq_t value = 0;
    for (idx_t pos = 0; pos < length; ++pos) {
        value = value * (seq_t)10
              + ((pos & (idx_t)1) == 0 ? (seq_t)a_digit : (seq_t)b_digit);
    }
    return value;
}
