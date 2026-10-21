/*
 * A204189  Benoît Perichon's 26 primes in arithmetic progression.
 * https://oeis.org/A204189
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = 43142746595714191 + 5283234035979900*(n-1)
 * Range:     u8 none, u16 none, u32 none, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Only 26 terms are defined in the sequence. */
    if (n < 1 || n > 26)
        return 0;

    const seq_t start = (seq_t)43142746595714191ULL;
    const seq_t step  = (seq_t)5283234035979900ULL;

    return start + step * (seq_t)(n - 1);
}
