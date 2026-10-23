/*
 * A103168  a(n) is the remainder when (n written backwards) is divided by n.
 * https://oeis.org/A103168
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = reverse(n) mod n
 * Range:     u8 n <= 296, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for OEIS sequence A103168: the remainder when
 * the decimal representation of n is reversed (written backwards)
 * and divided by n.  The result fits in seq_t for any n ≥ 1
 * and is computed without overflow using modular arithmetic. */
seq_t a(idx_t n)
{
    /* n is 1‑based, so n ≥ 1 when the function is called. */
    seq_t orig = (seq_t)n;           /* keep original n for modulo */
    seq_t rev_mod = 0;               /* current value of reversed digits modulo n */

    idx_t temp = n;                  /* work on this copy to extract digits */

    /* Build the reversed number digit by digit, taking the modulus
     * at each step to avoid overflow. */
    while (temp > 0) {
        rev_mod = (rev_mod * (seq_t)10 +
                   (seq_t)(temp % 10)) % orig;
        temp /= 10;
    }

    return rev_mod;
}
