/*
 * A037730  Base 4 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 2,1,0,3.
 * https://oeis.org/A037730
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = interpret the first n digits of the periodically repeated
 *            base‑4 digits 2, 1, 0, 3 as a base‑4 integer. For n ≥ 1 this
 *            yields the sequence A037730. The sequence satisfies a(n) =
 *            a(n-1) * 4 + d(n), where d(n) = {2, 1, 0, 3}[ (n-1) mod 4 ]
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* External linkage as required */
seq_t a(idx_t n)
{
    /* Periodic digits 2, 1, 0, 3 */
    static const seq_t pat[4] = {(seq_t)2, (seq_t)1, (seq_t)0, (seq_t)3};

    seq_t val = 0;
    for (idx_t i = 0; i < n; ++i)
        val = val * 4 + pat[i & 3];   /* i & 3 == i % 4 for period 4 */

    return val;
}
