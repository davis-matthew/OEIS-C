/*
 * A047342  Numbers that are congruent to {0, 3, 4} mod 7.
 * https://oeis.org/A047342
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(3k) = 7k‑3, a(3k‑1) = 7k‑4, a(3k‑2) = 7k‑7
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The function works for all n ≥ 1; n == 0 is never used but is
       harmless to handle. */
    if (n == 0) return 0;

    /* Reduce the index to k = ⌊(n+2)/3⌋.  The subtraction constant
       required to get the correct residue depends only on n mod 3. */
    seq_t k = (seq_t)((n + 2) / 3);

    switch ((int)(n % 3)) {
    case 0:  /* n = 3k0   → a = 7k – 3 */
        return (seq_t)7 * k - (seq_t)3;
    case 1:  /* n = 3k0+1 → a = 7k – 7 */
        return (seq_t)7 * k - (seq_t)7;
    case 2:  /* n = 3k0+2 → a = 7k – 4 */
        return (seq_t)7 * k - (seq_t)4;
    }
    /* Unreachable, but guarantees a return value. */
    return 0;
}
