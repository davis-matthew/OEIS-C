/*
 * A060728  Numbers k such that Ramanujan's equation x^2 + 7 = 2^k has an
 *          integer solution.
 * https://oeis.org/A060728
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   uses the finite list of known solutions of Ramanujan's equation
 *            x^2 + 7 = 2^k. The solutions for (x, k) are (1,3), (3,4), (5,5),
 *            (11,7), (181,15). Hence the sequence a(n) = k for n = 1..5
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* precomputed exponents k for the Ramanujan–Nagell equation */
    static const seq_t sol[] = { (seq_t)3, (seq_t)4, (seq_t)5,
                                 (seq_t)7, (seq_t)15 };
    const idx_t sz = sizeof(sol)/sizeof(sol[0]);

    if (n >= (idx_t)1 && n <= sz)
        return sol[n - (idx_t)1];
    /* sequence is finite; for indices beyond the known solutions return 0 */
    return (seq_t)0;
}
