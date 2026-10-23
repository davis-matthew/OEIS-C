/*
 * A138098  The Mendeleyev-Seaborg periodic table read from right to left
 *          (with spaces omitted).
 * https://oeis.org/A138098
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 97, u16 n <= 97, u32 n <= 97, u64 n <= 97, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements a(n) by reversing each period of the standard periodic table.
   The table has the following period limits (inclusive upper bounds):
   period 1  : 1–2  (2 elements)
   period 2  : 3–10 (8 elements)
   period 3  : 11–18 (8 elements)
   period 4  : 19–36 (18 elements)
   period 5  : 37–54 (18 elements)
   period 6  : 55–86 (32 elements)
   period 7  : 87–118 (32 elements)
   a(n) is the atomic number that lies at position n when each period is
   written from right to left.  For n > 118 the function returns 0.
*/
seq_t a(idx_t n)
{
    /* period cumulative upper bounds */
    static const idx_t limits[8] = {0, 2, 10, 18, 36, 54, 86, 118};

    if (n == 0 || n > limits[7]) {
        return (seq_t)0;
    }

    /* Find the period that contains n */
    idx_t p = 1;
    while (n > limits[p]) {
        ++p;
    }

    /* Period start (first element in reversed view) and end */
    idx_t start = limits[p - 1] + 1;
    idx_t end   = limits[p];

    /* Offset of n within this period (0‑based) */
    idx_t offset = n - start;

    /* Reversed value */
    return (seq_t)(end - offset);
}
