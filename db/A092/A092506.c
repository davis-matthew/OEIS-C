/*
 * A092506  Prime numbers of the form 2^n + 1.
 * https://oeis.org/A092506
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Prime numbers of the form 2^n + 1, i.e., the Fermat primes. */
seq_t a(idx_t n)
{
    /* Known Fermat primes: 2,3,5,17,257,65537 */
    static const seq_t fermat[] = {
        (seq_t)2,
        (seq_t)3,
        (seq_t)5,
        (seq_t)17,
        (seq_t)257,
        (seq_t)65537
    };
    /* Number of known terms */
    const idx_t count = sizeof(fermat) / sizeof(fermat[0]);

    /* n is 1‑based */
    if (n == 0 || n > count)
        return (seq_t)0;               /* undefined terms are 0 */

    return fermat[n - 1];
}
