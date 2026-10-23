/*
 * A194641  Digital roots of the nonzero heptagonal numbers.
 * https://oeis.org/A194641
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = A010888( A000566(n) ) 
 *            = digital_root( n*(5*n-3)/2 )
 *
 * The heptagonal number H(n) = n*(5n-3)/2 is always integer.
 * Its digital root is H(n) mod 9, with 0 mapped to 9.
 * We compute H(n) modulo 9 using only modulo 18 arithmetic
 * to avoid overflow for large n.
 */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined for the sequence offset but safe */

    /* Use 64‑bit arithmetic for modulo 18 calculations. */
    unsigned long long n_mod18 = (unsigned long long)(n % 18ULL);

    /* (5*n - 3) mod 18 == (5*n + 15) mod 18 */
    unsigned long long term = (5ULL * n_mod18 + 15ULL) % 18ULL;

    /* numerator modulo 18 */
    unsigned long long num = (n_mod18 * term) % 18ULL;

    /* numerator is even, divide by 2 to get H(n) mod 9 */
    unsigned long long h_mod9 = (num / 2ULL) % 9ULL;

    /* map remainder 0 to digital root 9 */
    if (h_mod9 == 0ULL)
        h_mod9 = 9ULL;

    return (seq_t)h_mod9;
}
