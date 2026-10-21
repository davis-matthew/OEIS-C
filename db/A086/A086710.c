/*
 * A086710  Position on a standard U.S. keyboard where the n-th letter of the
 *          alphabet can be found.
 * https://oeis.org/A086710
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   a(n) = the position of the n-th letter of the alphabet on a
 *            standard U.S. keyboard, scanned left‑to‑right, top‑to‑bottom,
 *            starting at Q = 1
 * Range:     u8 n <= 26, u16 n <= 26, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const char layout[] = "QWERTYUIOPASDFGHJKLZXCVBNM";
    idx_t idx  = (n - 1) % 26;            /* index of the nth letter 0‑based */
    char   letter = 'A' + (unsigned)idx;  /* actual letter to look up */
    for (idx_t i = 0; i < 26; ++i)
        if (layout[i] == letter)
            return (seq_t)(i + 1);        /* keys are 1‑based */
    return (seq_t)0; /* unreachable */
}
